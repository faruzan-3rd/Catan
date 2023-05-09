#include "catan/events.hpp"


void ctn::EventManager::update(){
    
}


bool ctn::EventManager::select(SelectionType type, int id, const str& color){
    GameState& state = progress->gamestate;
    if(type == SelectionType::Path){
        int req_settlement = (state.setup_phase ? state.last_placed_settlement : -1);
        bool ok = (state.setup_phase && state.can_place_path) || state.can_place_path;
        if(ok && board->can_build_path_here(color, id, req_settlement)){
            state.selected_id = id;
            state.selection_type = type;
            graphics->set_preview_build(color, type, id, board);
            return true;
        }

        return false;
    }
    else if(type == SelectionType::Settlement){
        bool ok = (state.setup_phase && state.can_place_settlement) || state.can_place_settlement;
        if(ok && board->can_build_settlement_here(color, id, state.setup_phase)){
            state.selected_id = id;
            state.selection_type = type;
            graphics->set_preview_build(color, type, id, board);
            return true;
        }

        return false;
    }
    return false;
}


bool ctn::EventManager::build_validate(){
    const GameState& state = progress->gamestate;
    str prefix = (state.setup_phase ? "setup_" : "");
    str building_type = (state.selection_type == SelectionType::Settlement ? "validate_settlement" : "validate_path");

    if(progress->execute_transition(prefix + building_type)){
        graphics->set_preview_build(ctn::INVISIBLE, SelectionType::None, -1, board);
        return true;
    }

    return false;
}