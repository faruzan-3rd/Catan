#include "catan/progress.hpp"




void ctn::ProgressManager::tick(Board* board){
    if(message.context == "setup" && message.player != -1){
        was::MouseManager* mouse = &(eventmgr->mousemanager);
        vec2f mousepos = mouse->get_mouse_pos_relative();
        if(mouse->button_pressed_down(sf::Mouse::Left)){
            int id;
            if(message.what == ctn::HOUSE){
                id = board->get_clicked_place(mousepos);
                if(id == -1) return;
                board->make_building_at(id, players[message.player].get_color() + ctn::HOUSE);
            }else{
                id = board->get_clicked_path(mousepos);
                if(id == -1) return;
                board->make_path_at(id, players[message.player].get_color());
            }
            
            message.verb = "placed";
            std::cout << "Player " << message.player << " placed a " << message.what << std::endl;    
        }
    }
}