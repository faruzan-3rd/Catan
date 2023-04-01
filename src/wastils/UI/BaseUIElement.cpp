#include "wastils/UI.hpp"


was::BaseUIElement::BaseUIElement(){
    window_ptr = nullptr;
}

was::BaseUIElement::BaseUIElement(sf::RenderWindow* window){
    window_ptr = window;
}
