#include "wastils/events.hpp"


void was::MouseManager::tick(const sf::Event& event){
    button_down.clear();

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

vec2f was::MouseManager::get_mouse_pos_relative(){
    return (vec2f)sf::Mouse::getPosition(*window);
}

void was::KeyboardManager::tick(const sf::Event& event){

}


bool was::EventManager::tick(){
    sf::Event event;
    while (window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed){
            window->close();
            return false;
        }

        mousemanager.tick(event);
        keyboardmanager.tick(event);
    }

    return true;
}