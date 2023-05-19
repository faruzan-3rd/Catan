#include "catan/progress.hpp"

#pragma region utilities
ctn::ProgressManager* progress;

ctn::ProgressManager::ProgressManager(){
    progress = this;
}

void set_player_turn_txt(was::UIScheme* ui, int player_num){
    std::stringstream sstream;
    sstream << "Player " << player_num << "'s turn";

    ui->get_ptr_by_name("player_turn")->set_text(sstream.str());
}

void set_order_txt(was::UIScheme* ui, const str& message){
    ui->get_ptr_by_name("order")->set_text(message);
}

bool is_valid_settlement(bool setup){
    const ctn::GameState& state = progress->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Settlement) return false;

    const str& color = progress->get_current_color();
    int selected_id = state.selected_id;

    return progress->board->can_build_settlement_here(color, selected_id, setup);
}

bool is_valid_path(){
    const ctn::GameState& state = progress->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Path) return false;

    const str& color = progress->get_current_color();
    int selected_id = state.selected_id;
    int req_settlement = (state.setup_phase ? state.last_placed_settlement : -1);
    return progress->board->can_build_path_here(color, selected_id, req_settlement);
}

bool is_valid_tile(){
    if(progress->gamestate.selection_type != ctn::SelectionType::Tile) return false;

    return progress->gamestate.selected_id != progress->board->get_robber_tile();
}

void enable_ui(const str& name){
    progress->ui->get_ptr_by_name(name)->enable(true);
}

void disable_ui(const str& name){
    progress->ui->get_ptr_by_name(name)->enable(false);
}

void modify_board_control(int mask){
    progress->gamestate.can_place_settlement = mask & ctn::SETTLEMENT;
    progress->gamestate.can_place_path = mask & ctn::PATH;
    progress->gamestate.can_select_tile = mask & ctn::TILE;
}

int find_next_robbed_player(){
    ctn::GameState& state = progress->gamestate;
    ctn::PlayersManager* playersmng = progress->playersmng;
    int players_num = playersmng->get_players_num();

    for(int player = state.current_player; player < players_num; player++){
        if(playersmng->get_player_info(player).get_total_resources() >= 7){
            return player;
        }
    }

    return -1;
}

bool resource_go_next(){
    ctn::GameState& state = progress->gamestate;
    ctn::PlayersManager* playersmng = progress->playersmng;
    int robbed_player = find_next_robbed_player();
    if(robbed_player == -1){
        progress->execute_transition("nobody_to_rob");
        return false;
    }

    int total_resouces = playersmng->get_player_info(robbed_player).get_total_resources();
    state.current_player = robbed_player;
    state.resource_select_num = total_resouces / 2;
    state.initialize_resource_selection();
    state.resource_select = true;
    return true;
}

void rob(){
    ctn::GameState& state = progress->gamestate;
    ctn::PlayersManager* playersmng = progress->playersmng;

    for(const str& mat : ctn::materials){
        playersmng->give_resource(state.current_player, mat, -1 * state.selected_resources[mat]);
    }
}
#pragma endregion


void on_start(){
    set_player_turn_txt(progress->ui, progress->gamestate.current_player);
    disable_ui("roll_dice");
    disable_ui("select_tile");
    disable_ui("confirm_dices");
    modify_board_control(ctn::SETTLEMENT);
}

bool is_valid_resource_selection(bool exit_transition){
    const ctn::GameState& state = progress->gamestate;

    if(!((state.current_player != progress->playersmng->get_players_num() - 1) ^ exit_transition)){
        return false;
    }

    if(state.selected_total_resources() == state.resource_select_num){
        rob();
        return true;
    }
    else{
        return false;
    }
}

void interpret_dices(){
    const ctn::GameState& state = progress->gamestate;
    std::cout << "+++++Dices rolled+++++" << std::endl;
    int 
        dice1 = state.dice1_result,
        dice2 = state.dice2_result;
    progress->graphics->enable_dice(dice1, dice2);
    progress->ui->get_ptr_by_name("roll_dice")->enable(false);

    std::cout << "Dice1: " << dice1 << ", Dice2: " << dice2 << std::endl;
    if(dice1 + dice2 == 7){
        std::cout << "ROBBER ACTIVATED" << std::endl;
    }
    else{
        std::cout << "RESOURCES" << std::endl;
    }

    enable_ui("confirm_dices");
}

void build_path(){
    ctn::GameState& state = progress->gamestate;
    int players_num = progress->playersmng->get_players_num();
    progress->board->build_path(state.selected_id, progress->get_current_color());

    // IF SETUP PHASE
    if(state.setup_phase){
        state.finished_players_num++;
        if(state.finished_players_num == players_num){
            if(state.second_tour){
                state.setup_phase = false;
                set_order_txt(progress->ui, "Roll dices");
                enable_ui("roll_dice");
                disable_ui("validate_build");
                modify_board_control(0);
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

        modify_board_control(ctn::SETTLEMENT);
        set_player_turn_txt(progress->ui, state.current_player);
        set_order_txt(progress->ui, "Build a settlement");
    }
    else{
        // progress.current_player++;
        // progress.current_player %= players.size();
    }
}

void build_settlement(){
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

        modify_board_control(ctn::PATH);

        set_order_txt(progress->ui, "Build a path");
    }
}

void to_roll_dice(){
    ctn::GameState& state = progress->gamestate;
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_int_distribution<int> rand(1, 6);
    // state.dice1_result = rand(engine);
    // state.dice2_result = rand(engine);  
    state.dice1_result = 3;
    state.dice2_result = 4;  
}

void to_select_tile(){
    disable_ui("confirm_dices");
    enable_ui("select_tile");
    modify_board_control(ctn::TILE);
}

void to_resource_select(){
    ctn::GameState& state = progress->gamestate;
    state.tmp_current_player = state.current_player;
    state.current_player = 0;
    modify_board_control(0);
    disable_ui("select_tile");
    progress->graphics->set_preview_build(ctn::NONE, ctn::SelectionType::None, -1, progress->board);
    progress->board->set_robber_location(state.selected_id);
    std::cout << "Tile selected" << std::endl;

    resource_go_next();
}

void to_resource_next_player(){
    resource_go_next();
}

void on_enter_trade(){
    std::cout << "++++ TRADE ++++" << std::endl;
    progress->gamestate.resource_select = false;
}

void ctn::ProgressManager::build_game_logic(){
    auto print = [](const std::string& msg){std::cout << msg << std::endl; };
    auto empty = [](){};
    add_state(State("start", empty));
    add_state(State("setup_settlement", empty));
    add_state(State("setup_path", empty));
    add_state(State("dice_roll", interpret_dices));
    add_state(State("select_tile", std::bind(print, "+++++Select tile+++++")));
    add_state(State("select_trade_action", on_enter_trade));
    add_state(State("robber_tile_select", to_select_tile));
    add_state(State("robber_resource_select", std::bind(print, "+++++Select resources+++")));

    add_transition(Transition(
        "start",
        "start",
        "setup_settlement",
        [](){return true;},
        on_start
    ));

    add_transition(Transition(
        "setup_validate_settlement", 
        "setup_settlement", 
        "setup_path", 
        std::bind(is_valid_settlement, true),
        build_settlement));

    add_transition(Transition(
        "setup_validate_path", 
        "setup_path", 
        "setup_settlement", 
        is_valid_path,
        build_path));

    add_transition(Transition(
        "roll_dices",
        "setup_settlement",
        "dice_roll",
        [this](){return !this->gamestate.setup_phase;},
        to_roll_dice
    ));

    add_transition(Transition(
        "enter_tile_select",
        "dice_roll",
        "robber_tile_select",
        [this](){return this->gamestate.dice1_result + this->gamestate.dice2_result == 7; },
        empty
    ));

    add_transition(Transition(
        "enter_resource_select",
        "robber_tile_select",
        "robber_resource_select",
        is_valid_tile,
        to_resource_select
    ));

    add_transition(Transition(
        "go_next_player_robber",
        "robber_resource_select",
        "robber_resource_select",
        std::bind(is_valid_resource_selection, false),
        to_resource_next_player
    ));

    add_transition(Transition(
        "enter_trade_action_select_from_robber",
        "robber_resource_select",
        "select_trade_action",
        std::bind(is_valid_resource_selection, true),
        [this](){this->gamestate.current_player = this->gamestate.tmp_current_player; }
    ));

    add_transition(Transition(
        "nobody_to_rob",
        "robber_resource_select",
        "select_trade_action",
        [](){return true; },
        empty
    ));

    add_transition(Transition(
        "enter_trade_action_select",
        "dice_roll",
        "select_trade_action",
        [this](){return this->gamestate.dice1_result + this->gamestate.dice2_result != 7; },
        empty
    ));
}