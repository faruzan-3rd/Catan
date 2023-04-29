#include "wastils/input.hpp"


void was::MouseManager::tick(){
    button_down.clear();
    for(sf::Mouse::Button btn : buttons){
        button_down[btn] = false;
    }
}

void was::MouseManager::reg_event(const sf::Event& event){
    for(sf::Mouse::Button btn : buttons){
        if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == btn && lock_click[btn] != true) {
                lock_click[btn] = true;
                button_down[btn] = true;
            }   
        }

        if (event.type == sf::Event::MouseButtonReleased){
            if (event.mouseButton.button == btn){
                lock_click[btn] = false; 
            }
        }
    }
}

vec2f was::MouseManager::get_mouse_pos_relative() const{
    return (vec2f)sf::Mouse::getPosition(*window);
}

void was::KeyboardManager::tick(const sf::Event& event){
    
}
