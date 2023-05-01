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
    if(progress->selection_type != ctn::PLACE) return false;

    return progress->board->can_build_settlement_here(players[progress->current_player].get_color(), progress->selected_id, setup);
}

bool is_valid_path(ctn::ProgressManager* progress){
    if(progress->selection_type != ctn::PATH) return false;

    auto selected_path = progress->selected_id;
    auto current_color = players[progress->current_player].get_color();
    int req_settlement = (progress->gamestate.setup_phase ? progress->gamestate.last_placed_settlement : -1);
    return progress->board->can_build_path_here(current_color, selected_path, req_settlement);
}
#pragma endregion


void on_start(ctn::ProgressManager* progress){
    set_player_turn_txt(progress->ui, progress->current_player);
    progress->ui->get_ptr_by_name("roll_dice")->enable(false);
}

void build_path(ctn::ProgressManager* progress){
    progress->board->build_path(progress->selected_id, players[progress->current_player].get_color());

    // IF SETUP PHASE
    if(progress->gamestate.setup_phase){
        progress->gamestate.can_place_path = false;
        progress->gamestate.finished_players_num++;
        if(progress->gamestate.finished_players_num == (int)players.size()){
            if(progress->gamestate.second_tour){
                progress->gamestate.setup_phase = false;
                set_order_txt(progress->ui, "Roll dices");
                progress->ui->get_ptr_by_name("roll_dice")->enable(true);
                progress->ui->get_ptr_by_name("validate_build")->enable(false);
                return;
            }
            else{
                progress->gamestate.second_tour = true;
                progress->gamestate.finished_players_num = 0;
            }
        }
        else{
            progress->current_player = (progress->current_player + (progress->gamestate.second_tour ? -1 : 1) + players.size()) % players.size();
        }

        progress->gamestate.can_place_settlement = true;
        set_player_turn_txt(progress->ui, progress->current_player);
        set_order_txt(progress->ui, "Build a settlement");
    }
    else{
        // progress->current_player++;
        // progress->current_player %= players.size();
    }
}


void build_settlement(ctn::ProgressManager* progress){
    progress->board->build_settlement(progress->selected_id, ctn::HOUSE, players[progress->current_player].get_color());
    progress->gamestate.last_placed_settlement = progress->selected_id;

    if(progress->gamestate.setup_phase){
        if(progress->gamestate.second_tour){
            std::cout << "Generate resource!" << std::endl;
        }

        progress->gamestate.can_place_path = true;
        progress->gamestate.can_place_settlement = false;

        set_order_txt(progress->ui, "Build a path");
    }
}

void roll_dice(ctn::ProgressManager* progress){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_int_distribution<int> rand(1, 6);
    progress->gamestate.dice1_result = rand(engine);
    progress->gamestate.dice2_result = rand(engine);  
}

void interpret_dices(ctn::ProgressManager* progress){
    std::cout << "+++++Dices rolled+++++" << std::endl;
    int 
        dice1 = progress->gamestate.dice1_result,
        dice2 = progress->gamestate.dice2_result;
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
    add_state(State("start", [](){}));
    add_state(State("setup_settlement", std::bind(print, "setup settlement")));
    add_state(State("setup_path", std::bind(print, "setup path")));
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