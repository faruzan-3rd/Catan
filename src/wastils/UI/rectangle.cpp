#include "wastils/UI.hpp"


was::Rectangle::Rectangle(){

}

was::Rectangle::Rectangle(const sf::IntRect& rect, const sf::Color& color, const int thickness, sf::RenderWindow* window){
    window_ptr = window;
    rectangle = sf::RectangleShape(sf::Vector2f(rect.width, rect.height));
    rectangle.setPosition(sf::Vector2f(rect.left, rect.top));
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(thickness);
    rectangle.setFillColor(sf::Color::Transparent);
}

void was::Rectangle::set_position(sf::Vector2f pos){
    rectangle.setPosition(pos);
}

void was::Rectangle::set_color(const sf::Color& color){
    rectangle.setOutlineColor(color);
}

void was::Rectangle::set_thickness(int thickness){
    rectangle.setOutlineThickness(thickness);
}

sf::Vector2f was::Rectangle::get_position(){
    return rectangle.getPosition();
}

void was::Rectangle::draw(){
    window_ptr->draw(rectangle);
}