#ifndef __WASTILS_INPUT_HPP
#define __WASTILS_INPUT_HPP

#include "SFML/Graphics.hpp"
#include <map>

typedef sf::Vector2f vec2f;

namespace was{
    class MouseManager{
        sf::RenderWindow* window;
        const std::vector<sf::Mouse::Button> buttons = {
            sf::Mouse::Left,
            sf::Mouse::Right,
            sf::Mouse::Middle
        };
        std::map<sf::Mouse::Button, bool> 
            lock_click,
            button_down;

        public:
        MouseManager():
            window{nullptr}
        {};

        MouseManager(sf::RenderWindow* window_):
            window{window_}
        {};

        MouseManager& operator=(const MouseManager& other){
            window = other.window;
            return *this;
        }

        void tick();
        void reg_event(const sf::Event& event);
        bool button_pressed_down(const sf::Mouse::Button& btn) const{ return button_down.at(btn); }
        vec2f get_mouse_pos_relative() const;

    };

    class KeyboardManager{

        public:
        void tick(const sf::Event& event);
    };
}


#endif