#include "catan/phases/common.hpp"
using namespace ctn::phases;
extern ctn::ProgressManager* prog_ptr;

#pragma region UI_STUFF
void common::set_player_turn_txt(was::UIScheme* ui, int player_num){
    std::stringstream sstream;
    sstream << "Player " << player_num << "'s turn";

    ui->get_ptr_by_name("player_turn")->set_text(sstream.str());
}

void common::set_order_txt(was::UIScheme* ui, const str& message){
    ui->get_ptr_by_name("order")->set_text(message);
}

void common::enable_ui(const str& name){
    prog_ptr->ui->get_ptr_by_name(name)->enable(true);
}

void common::disable_ui(const str& name){
    prog_ptr->ui->get_ptr_by_name(name)->enable(false);
}

void common::modify_board_control(int mask){
    prog_ptr->gamestate.can_place_settlement = mask & ctn::SETTLEMENT;
    prog_ptr->gamestate.can_place_path = mask & ctn::PATH;
    prog_ptr->gamestate.can_select_tile = mask & ctn::TILE;
}
#pragma endregion


#pragma region CONDITIONS
bool common::is_valid_settlement(bool setup){
    const ctn::GameState& state = prog_ptr->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Settlement) return false;

    const str& color = prog_ptr->get_current_color();
    int selected_id = state.selected_id;

    return prog_ptr->board->can_build_settlement_here(color, selected_id, setup);
}

bool common::is_valid_path(){
    const ctn::GameState& state = prog_ptr->gamestate;
    ctn::SelectionType type = state.selection_type;
    if(type != ctn::SelectionType::Path) return false;

    const str& color = prog_ptr->get_current_color();
    int selected_id = state.selected_id;
    int req_settlement = (state.setup_phase ? state.last_placed_settlement : -1);
    return prog_ptr->board->can_build_path_here(color, selected_id, req_settlement);
}

bool common::is_valid_tile(){
    if(prog_ptr->gamestate.selection_type != ctn::SelectionType::Tile) return false;

    return prog_ptr->gamestate.selected_id != prog_ptr->board->get_robber_tile();
}
#pragma endregion



void common::build_path(){
    ctn::GameState& state = prog_ptr->gamestate;
    int players_num = prog_ptr->playersmng->get_players_num();
    prog_ptr->board->build_path(state.selected_id, prog_ptr->get_current_color());

    // IF SETUP PHASE
    if(state.setup_phase){
        state.finished_players_num++;
        if(state.finished_players_num == players_num){
            if(state.second_tour){
                state.setup_phase = false;
                set_order_txt(prog_ptr->ui, "Roll dices");
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
        set_player_turn_txt(prog_ptr->ui, state.current_player);
        set_order_txt(prog_ptr->ui, "Build a settlement");
    }
    else{
        // progress.current_player++;
        // progress.current_player %= players.size();
    }
}

void common::build_settlement(){
    ctn::GameState& state = prog_ptr->gamestate;
    int selected = state.selected_id;
    prog_ptr->board->build_settlement(selected, ctn::HOUSE, prog_ptr->get_current_color());
    state.last_placed_settlement = selected;

    if(state.setup_phase){
        if(state.second_tour){
            for(const str& resource : prog_ptr->board->get_obtainable_resources(selected)){
                prog_ptr->playersmng->give_resource(state.current_player, resource);
            }
        }

        modify_board_control(ctn::PATH);

        set_order_txt(prog_ptr->ui, "Build a path");
    }
}