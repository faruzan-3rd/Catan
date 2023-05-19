#include "catan/phases/early.hpp"
#include "catan/phases/common.hpp"
using namespace ctn::phases;
extern ctn::ProgressManager* prog_ptr;

#pragma region UTILITIES
int early::find_next_robbed_player(){
    ctn::GameState& state = prog_ptr->gamestate;
    ctn::PlayersManager* playersmng = prog_ptr->playersmng;
    int players_num = playersmng->get_players_num();

    for(int player = state.current_player; player < players_num; player++){
        if(playersmng->get_player_info(player).get_total_resources() >= 7){
            return player;
        }
    }

    return -1;
}

bool early::resource_go_next(){
    ctn::GameState& state = prog_ptr->gamestate;
    ctn::PlayersManager* playersmng = prog_ptr->playersmng;
    int robbed_player = find_next_robbed_player();
    if(robbed_player == -1){
        prog_ptr->execute_transition("nobody_to_rob");
        return false;
    }

    int total_resouces = playersmng->get_player_info(robbed_player).get_total_resources();
    state.current_player = robbed_player;
    state.resource_select_num = total_resouces / 2;
    state.initialize_resource_selection();
    state.resource_select = true;
    return true;
}

void early::rob(){
    ctn::GameState& state = prog_ptr->gamestate;
    ctn::PlayersManager* playersmng = prog_ptr->playersmng;

    for(const str& mat : ctn::materials){
        playersmng->give_resource(state.current_player, mat, -1 * state.selected_resources[mat]);
    }
}
#pragma endregion



bool early::is_valid_resource_selection(bool exit_transition){
    const ctn::GameState& state = prog_ptr->gamestate;

    if(!((state.current_player != prog_ptr->playersmng->get_players_num() - 1) ^ exit_transition)){
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


#pragma region STATE_FUNCTIONS
void early::on_start(){
    common::set_player_turn_txt(prog_ptr->ui, prog_ptr->gamestate.current_player);
    common::disable_ui("roll_dice");
    common::disable_ui("select_tile");
    common::disable_ui("confirm_dices");
    common::modify_board_control(ctn::SETTLEMENT);
}

void early::interpret_dices(){
    const ctn::GameState& state = prog_ptr->gamestate;
    std::cout << "+++++Dices rolled+++++" << std::endl;
    int 
        dice1 = state.dice1_result,
        dice2 = state.dice2_result;
    prog_ptr->graphics->enable_dice(dice1, dice2);
    prog_ptr->ui->get_ptr_by_name("roll_dice")->enable(false);

    std::cout << "Dice1: " << dice1 << ", Dice2: " << dice2 << std::endl;
    if(dice1 + dice2 == 7){
        std::cout << "ROBBER ACTIVATED" << std::endl;
    }
    else{
        std::cout << "RESOURCES" << std::endl;
    }

    common::enable_ui("confirm_dices");
}

#pragma endregion



#pragma region CALLBACKS
void early::to_roll_dice(){
    ctn::GameState& state = prog_ptr->gamestate;
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::uniform_int_distribution<int> rand(1, 6);
    // state.dice1_result = rand(engine);
    // state.dice2_result = rand(engine);  
    state.dice1_result = 3;
    state.dice2_result = 4;  
}

void early::to_select_tile(){
    common::disable_ui("confirm_dices");
    common::enable_ui("select_tile");
    common::modify_board_control(ctn::TILE);
}

void early::to_resource_select(){
    ctn::GameState& state = prog_ptr->gamestate;
    state.tmp_current_player = state.current_player;
    state.current_player = 0;
    common::modify_board_control(0);
    common::disable_ui("select_tile");
    prog_ptr->graphics->set_preview_build(ctn::NONE, ctn::SelectionType::None, -1, prog_ptr->board);
    prog_ptr->board->set_robber_location(state.selected_id);
    std::cout << "Tile selected" << std::endl;

    resource_go_next();
}

void early::to_resource_next_player(){
    resource_go_next();
}
#pragma endregion