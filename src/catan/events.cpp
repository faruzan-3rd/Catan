#include "catan/events.hpp"


void ctn::EventManager::update(){
    if(!mouse->button_pressed_down(sf::Mouse::Left)) return;

    const vec2f mouse_pos = mouse->get_mouse_pos_relative();
    int 
        clicked_path = board->get_clicked_path(mouse_pos),
        clicked_settlement = board->get_clicked_place(mouse_pos);

    if(clicked_path != -1){
        if((progress->gamestate.setup_phase && progress->gamestate.can_place_path) || progress->gamestate.can_place_path){
            progress->selected_id = clicked_path;
            progress->selection_type = ctn::PATH;
            graphics->set_preview_build(players[progress->current_player].get_color(), ctn::PATH, clicked_path, board, progress->gamestate.setup_phase);
            std::cout << "Selected path " << clicked_path << std::endl;
        }
    }
    if(clicked_settlement != -1){
        if((progress->gamestate.setup_phase && progress->gamestate.can_place_settlement) || progress->gamestate.can_place_settlement){
            progress->selected_id = clicked_settlement;
            progress->selection_type = ctn::PLACE;
            graphics->set_preview_build(players[progress->current_player].get_color(), ctn::PLACE, clicked_settlement, board, progress->gamestate.setup_phase);
            std::cout << "Selected settlement " << clicked_settlement << std::endl;
        }
    }
}


void ctn::EventManager::build_validate(){
    str prefix = (progress->gamestate.setup_phase ? "setup_" : "");
    str building_type = (progress->selection_type == ctn::PLACE ? "validate_settlement" : "validate_path");

    if(progress->execute_transition(prefix + building_type)){
        graphics->set_preview_build(ctn::INVISIBLE, -1, -1, board, false);
    }
}