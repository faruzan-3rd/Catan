#include "wastils/UI.hpp"


was::Text::Text(){
    text = sf::Text();
}

was::Text::Text(const std::string& text_str, sf::Font font, sf::RenderWindow* window, const int character_size, const sf::Color color, const sf::Vector2f position){
    this->text = sf::Text();
    this->font = font;
    this->window_ptr = window;

    text.setString(text_str);
    text.setFont(this->font);
    text.setFillColor(color);
    text.setPosition(position);
    text.setCharacterSize(character_size);
}

void was::Text::set_text(const std::string& text){
    this->text.setString(text);
}

void was::Text::set_character_size(const int size){
    this->text.setCharacterSize(size);
}

void was::Text::set_color(const sf::Color& color){
    this->text.setFillColor(color);
}

void was::Text::set_font(const sf::Font font){
    this->font = font;
    text.setFont(this->font);
}

void was::Text::draw(){
    window_ptr->draw(text);
}

void was::Text::set_position(sf::Vector2f pos){
    this->text.setPosition(pos);
}