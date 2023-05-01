#include "catan/events.hpp"


void ctn::EventManager::update(){
    if(!mouse->button_pressed_down(sf::Mouse::Left)) return;

    const vec2f mouse_pos = mouse->get_mouse_pos_relative();
    int 
        clicked_path = board->get_clicked_path(mouse_pos),
        clicked_settlement = board->get_clicked_place(mouse_pos);
    
    str color = players[progress->current_player].get_color();

    if(clicked_path != -1){
        int req_settlement = (progress->gamestate.setup_phase ? progress->gamestate.last_placed_settlement : -1);
        bool ok = (progress->gamestate.setup_phase && progress->gamestate.can_place_path) || progress->gamestate.can_place_path;
        if(ok && board->can_build_path_here(color, clicked_path, req_settlement)){
            progress->selected_id = clicked_path;
            progress->selection_type = ctn::PATH;
            graphics->set_preview_build(color, ctn::PATH, clicked_path, board);
        }
    }
    if(clicked_settlement != -1){
        bool ok = (progress->gamestate.setup_phase && progress->gamestate.can_place_settlement) || progress->gamestate.can_place_settlement;
        if(ok && board->can_build_settlement_here(color, clicked_settlement, progress->gamestate.setup_phase)){
            progress->selected_id = clicked_settlement;
            progress->selection_type = ctn::PLACE;
            graphics->set_preview_build(players[progress->current_player].get_color(), ctn::PLACE, clicked_settlement, board);
        }
    }
}


void ctn::EventManager::build_validate(){
    str prefix = (progress->gamestate.setup_phase ? "setup_" : "");
    str building_type = (progress->selection_type == ctn::PLACE ? "validate_settlement" : "validate_path");

    if(progress->execute_transition(prefix + building_type)){
        graphics->set_preview_build(ctn::INVISIBLE, -1, -1, board);
    }
}