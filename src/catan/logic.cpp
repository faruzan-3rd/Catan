#include "catan/progress.hpp"

#pragma region utilities
void set_player_turn_txt(was::UIScheme* ui, int player_num){
    std::stringstream sstream;
    sstream << "Player " << player_num << "'s turn";

    ui->get_ptr_by_name("player_turn")->set_text(sstream.str());
}

void set_order_txt(was::UIScheme* ui, const str& message){
    ui->get_ptr_by_name("order")->set_text(message);
}

bool is_valid_settlement(ctn::ProgressManager* progress, bool setup){
    const ctn::GameState& state = progress->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Settlement) return false;

    const str& color = progress->get_current_color();
    int selected_id = state.selected_id;

    return progress->board->can_build_settlement_here(color, selected_id, setup);
}

bool is_valid_path(ctn::ProgressManager* progress){
    const ctn::GameState& state = progress->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Path) return false;

    const str& color = progress->get_current_color();
    int selected_id = state.selected_id;
    int req_settlement = (state.setup_phase ? state.last_placed_settlement : -1);
    return progress->board->can_build_path_here(color, selected_id, req_settlement);
}
#pragma endregion


void on_start(ctn::ProgressManager* progress){
    set_player_turn_txt(progress->ui, progress->gamestate.current_player);
    progress->ui->get_ptr_by_name("roll_dice")->enable(false);
}

void build_path(ctn::ProgressManager* progress){
    ctn::GameState& state = progress->gamestate;
    int players_num = progress->playersmng->get_players_num();
    progress->board->build_path(state.selected_id, progress->get_current_color());

    // IF SETUP PHASE
    if(state.setup_phase){
        state.can_place_path = false;
        state.finished_players_num++;
        if(state.finished_players_num == players_num){
            if(state.second_tour){
                state.setup_phase = false;
                set_order_txt(progress->ui, "Roll dices");
                progress->ui->get_ptr_by_name("roll_dice")->enable(true);
                progress->ui->get_ptr_by_name("validate_build")->enable(false);
                return;
            }
            else{
                state.second_tour = true;
                state.finished_players_num = 0;
            }
        }
        else{
            int rotation = (state.second_tour ? -1 : 1);
            state.current_player = (state.current_player + rotation + players_num) % players_num;
        }

        state.can_place_settlement = true;
        set_player_turn_txt(progress->ui, state.current_player);
        set_order_txt(progress->ui, "Build a settlement");
    }
    else{
        // progress->current_player++;
        // progress->current_player %= players.size();
    }
}


void build_settlement(ctn::ProgressManager* progress){
    ctn::GameState& state = progress->gamestate;
    int selected = state.selected_id;
    progress->board->build_settlement(selected, ctn::HOUSE, progress->get_current_color());
    state.last_placed_settlement = selected;

    if(state.setup_phase){
        if(state.second_tour){
            for(const str& resource : progress->board->get_obtainable_resources(selected)){
                progress->playersmng->give_resource(state.current_player, resource);
            }
        }

        state.can_place_path = true;
        state.can_place_settlement = false;

        set_order_txt(progress->ui, "Build a path");
    }
}

void roll_dice(ctn::ProgressManager* progress){
    ctn::GameState& state = progress->gamestate;
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_int_distribution<int> rand(1, 6);
    state.dice1_result = rand(engine);
    state.dice2_result = rand(engine);  
}

void interpret_dices(ctn::ProgressManager* progress){
    const ctn::GameState& state = progress->gamestate;
    std::cout << "+++++Dices rolled+++++" << std::endl;
    int 
        dice1 = state.dice1_result,
        dice2 = state.dice2_result;
    progress->graphics->enable_dice(dice1, dice2);

    std::cout << "Dice1: " << dice1 << ", Dice2: " << dice2 << std::endl;
    if(dice1 + dice2 == 7){
        std::cout << "ROBBER ACTIVATED" << std::endl;
    }
    else{
        std::cout << "RESOURCES" << std::endl;
    }
}


void ctn::ProgressManager::build_game_logic(){
    auto print = [](const std::string& msg){std::cout << msg << std::endl; };
    auto empty = [](){};
    add_state(State("start", empty));
    add_state(State("setup_settlement", empty));
    add_state(State("setup_path", empty));
    add_state(State("dice_roll", [this](){interpret_dices(this);}));
    add_state(State("select_tile", std::bind(print, "+++++Select tile+++++")));
    add_state(State("select_trade_action", std::bind(print, "+++++Trade+++++")));

    add_transition(Transition(
        "start",
        "start",
        "setup_settlement",
        [](){return true;},
        [this](){on_start(this);}
    ));

    add_transition(Transition(
        "setup_validate_settlement", 
        "setup_settlement", 
        "setup_path", 
        std::bind(is_valid_settlement, this, true),
        std::bind(build_settlement, this)));

    add_transition(Transition(
        "setup_validate_path", 
        "setup_path", 
        "setup_settlement", 
        std::bind(is_valid_path, this),
        std::bind(build_path, this)));

    add_transition(Transition(
        "roll_dices",
        "setup_settlement",
        "dice_roll",
        [this](){return !this->gamestate.setup_phase;},
        [this](){roll_dice(this);}
    ));
}