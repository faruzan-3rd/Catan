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

        void tick(const sf::Event& event);
        bool button_pressed_down(const sf::Mouse::Button& btn){ return button_down[btn]; }
        vec2f get_mouse_pos_relative();

    };

    class KeyboardManager{

        public:
        void tick(const sf::Event& event);
    };


    class EventManager{
        sf::RenderWindow* window;

        public:
        MouseManager mousemanager;
        KeyboardManager keyboardmanager;

        EventManager():
            window{nullptr},
            mousemanager{},
            keyboardmanager{}
        {}

        EventManager(sf::RenderWindow* window_):
            window{window_},
            mousemanager{window_},
            keyboardmanager{}
        {}

        bool tick();
    };
}


#endif