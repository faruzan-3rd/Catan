#ifndef __WAS__UI_HPP
#define __WAS__UI_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"


namespace was{

    class BaseUIElement{
        private:
        sf::RenderWindow* window_ptr;

        public:
        BaseUIElement();
        BaseUIElement(sf::RenderWindow* window);

        virtual void set_text(const std::string& text){}

        virtual void set_character_size(const int size){}

        virtual void set_color(const sf::Color& color){}

        virtual void set_font(sf::Font font){}

        virtual void set_position(sf::Vector2f pos){}

        virtual void set_thickness(int thickness){}

        virtual void draw(){}

        virtual sf::Vector2f get_position(){ return sf::Vector2f(0, 0); }
    };


    class Text: public BaseUIElement{
        private:

        sf::Text text;
        sf::Font font;

        sf::RenderWindow* window_ptr;


        public:
        Text();
        Text(const std::string& text_str, sf::Font font, sf::RenderWindow* window, const int character_size, const sf::Color color, const sf::Vector2f position);

        void set_text(const std::string& text) override;

        void set_character_size(const int size) override;

        void set_color(const sf::Color& color) override;

        void set_font(sf::Font font) override;

        void set_position(sf::Vector2f pos) override;

        void draw() override;
    };


    class Rectangle: public BaseUIElement{
        private:
        sf::RectangleShape rectangle;
        sf::RenderWindow* window_ptr;

        public:
        Rectangle();
        Rectangle(const sf::IntRect& rect, const sf::Color& color, const int thickness, sf::RenderWindow* window);

        void set_position(sf::Vector2f pos) override;

        void set_color(const sf::Color& color) override;

        void set_thickness(int thickness) override;

        void draw() override;

        sf::Vector2f get_position() override;
    };



    class UIScheme{
        private:
        std::map<std::string, BaseUIElement*> element_ptrs;

        sf::RenderWindow* window_ptr;

        YAML::Node scheme_src;

        public:
        UIScheme();
        UIScheme(const std::string& scheme_location, sf::RenderWindow* window);
        UIScheme(const UIScheme& scheme);
        ~UIScheme();

        UIScheme& operator =(const UIScheme& other); 

        void load(const YAML::Node& node);

        void draw();

        BaseUIElement* get_ptr_by_name(const std::string& name);


        private:
        Text* load_text(const YAML::Node& node);
        Rectangle* load_rectangle(const YAML::Node& node);
    };
}



#endif