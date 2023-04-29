#include "catan/events.hpp"


void ctn::EventManager::update(){
    if(!mouse->button_pressed_down(sf::Mouse::Left)) return;

    const vec2f mouse_pos = mouse->get_mouse_pos_relative();
    int 
        clicked_path = board->get_clicked_path(mouse_pos),
        clicked_settlement = board->get_clicked_place(mouse_pos);

    if(clicked_path != -1){
        progress->selected_id = clicked_path;
        progress->selection_type = ctn::PATH;
        std::cout << "Selected path " << clicked_path << std::endl;
    }
    if(clicked_settlement != -1){
        progress->selected_id = clicked_settlement;
        progress->selection_type = ctn::PLACE;
        std::cout << "Selected settlement " << clicked_settlement << std::endl;
    }
}


void ctn::EventManager::build_validate(){
    str prefix = (progress->setupdata.setup_phase ? "setup_" : "");
    str building_type = (progress->selection_type == ctn::PLACE ? "validate_settlement" : "validate_path");

    std::cout << prefix + building_type << std::endl;
    progress->execute_transition(prefix + building_type);
}