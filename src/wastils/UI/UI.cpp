#include "wastils/UI.hpp"


void was::Button::update_(const was::MouseManager& mouse){
    sf::FloatRect rect{button_sprite.getGlobalBounds()};
    

    if(mouse.button_pressed_down(trigger_button) && rect.contains(mouse.get_mouse_pos_relative())){
        func();
    }
}