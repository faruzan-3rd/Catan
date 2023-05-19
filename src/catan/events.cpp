#include "catan/events.hpp"


void ctn::EventManager::update(){
    
}


bool ctn::EventManager::select(SelectionType type, int id, const str& color){
    if(id == -1) return false; 

    GameState& state = progress->gamestate;
    if(type == SelectionType::Path){
        int req_settlement = (state.setup_phase ? state.last_placed_settlement : -1);
        bool ok = state.can_place_path;
        if(ok && board->can_build_path_here(color, id, req_settlement)){
            state.selected_id = id;
            state.selection_type = type;
            graphics->set_preview_build(color, type, id, board);
            return true;
        }
    }
    else if(type == SelectionType::Settlement){
        bool ok = state.can_place_settlement;
        if(ok && board->can_build_settlement_here(color, id, state.setup_phase)){
            state.selected_id = id;
            state.selection_type = type;
            graphics->set_preview_build(color, type, id, board);
            return true;
        }
    }
    else if(type == SelectionType::Tile){
        if(state.can_select_tile){
            graphics->set_preview_build(ctn::NONE, ctn::SelectionType::Tile, id, board);
            state.selected_id = id;
            state.selection_type = type;

            return true;
        }
    }
    return false;
}

bool ctn::EventManager::confirm(){
    const GameState& state = progress->gamestate;
    if(state.selection_type == SelectionType::Path || state.selection_type == SelectionType::Settlement){
        str prefix = (state.setup_phase ? "setup_" : "");
        str building_type = (state.selection_type == SelectionType::Settlement ? "validate_settlement" : "validate_path");

        if(progress->execute_transition(prefix + building_type)){
            graphics->set_preview_build(ctn::INVISIBLE, SelectionType::None, -1, board);
            return true;
        }

        return false;
    }
    else if(state.selection_type == SelectionType::Tile){
        if(progress->execute_transition("enter_resource_select")){
            graphics->set_preview_build(ctn::NONE, ctn::SelectionType::None, -1, board);
            return true;
        }
    }

    return false;
}

bool ctn::EventManager::confirm_dices(){
    const GameState& state = progress->gamestate;

    if(state.dice1_result + state.dice2_result == 7){
        return progress->execute_transition("enter_tile_select");
    }
    else{
        return progress->execute_transition("enter_trade_action_select");
    }
}

void ctn::EventManager::add_resource(const str& resource){
    GameState& state = progress->gamestate;
    if(state.selected_total_resources() == state.resource_select_num){
        return;
    }

    int limit = progress->playersmng->get_player_info(state.current_player).get_resource_quantity(resource);
    int current = state.get_selected_resource(resource);
    if(current == limit){
        return;
    }

    state.selected_resources[resource] += 1;
    return;
}

void ctn::EventManager::remove_resource(const str& resource){
    GameState& state = progress->gamestate;
    int current = state.get_selected_resource(resource);
    if(current == 0){
        return;
    }

    state.selected_resources[resource] -= 1;
    return;
}