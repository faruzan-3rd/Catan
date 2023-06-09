#ifndef __WAS__UI_HPP
#define __WAS__UI_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"
#include "wastils/input.hpp"

using vec2f = sf::Vector2f;

namespace was{

    class BaseUIElement{
        protected:
        bool enabled;
        vec2f position;
        vec2f relative;
        sf::RenderWindow* window_ptr;

        public:
        BaseUIElement():
            enabled{true},
            relative{vec2f{0, 0}},
            window_ptr{nullptr}
        {}
        BaseUIElement(sf::RenderWindow* window):
            enabled{true},
            relative{vec2f{0, 0}},
            window_ptr{window}
        {}
        virtual ~BaseUIElement(){};

        virtual void set_text(const std::string& new_str){}

        virtual void set_character_size(const int size){}

        virtual void set_color(const sf::Color& color){}

        virtual void set_font(const sf::Font& font_){}

        virtual void set_position(const vec2f& pos){position = pos; }

        virtual void set_thickness(int thickness){}

        virtual void set_function(std::function<void()> func){}

        virtual void set_new_image(const sf::Texture& texture_, const sf::Sprite& sprite){}

        void draw(){if(enabled) draw_(); }

        virtual vec2f get_position(){ return position; }

        void update(const was::MouseManager& mouse){if(enabled) update_(mouse); }

        virtual void enable(const bool& enable_){enabled = enable_; }

        void set_relative(const vec2f& origin){relative = origin; }

        vec2f get_relative() const {return relative; }

        protected:
        virtual void update_(const was::MouseManager& mouse){}
        virtual void draw_(){}
    };


    class Text: public BaseUIElement{
        private:
        sf::Text text;
        sf::Font font;

        public:
        Text() = default;
        Text(
            const std::string& text_str, 
            sf::Font font_, sf::RenderWindow* window, 
            const int character_size, 
            const sf::Color color, 
            const vec2f position):

            BaseUIElement{window},
            text{text_str, font, (unsigned)character_size},
            font{font_}
        {
            text.setFillColor(color);
            set_position(position);
        }

        void set_text(const std::string& new_str) override{
            text.setString(new_str);
        }

        void set_character_size(const int size) override{
            text.setCharacterSize(size);
        }

        void set_color(const sf::Color& color) override{
            text.setFillColor(color);
        }

        void set_font(const sf::Font& font_) override{
            font = font_;
            text.setFont(font);
        }

        void draw_() override{
            text.setPosition(position + relative);
            window_ptr->draw(text);
        }
    };


    class Rectangle: public BaseUIElement{
        private:
        sf::RectangleShape rectangle;

        public:
        Rectangle() = default;
        Rectangle(
            const sf::IntRect& rect, 
            const sf::Color& color, 
            const int thickness, 
            sf::RenderWindow* window):

            BaseUIElement{window},
            rectangle{vec2f{(float)rect.left, (float)rect.top}}
        {}

        void set_color(const sf::Color& color) override{
            rectangle.setOutlineColor(color);
        }

        void set_thickness(int thickness) override{
            rectangle.setOutlineThickness(thickness);
        }

        void draw_() override{
            rectangle.setPosition(position + relative);
            window_ptr->draw(rectangle);
        }
    };

    class Image: public BaseUIElement{
        private:
        sf::Texture texture;
        sf::Sprite sprite;

        public:
        Image() = default;
        Image(const sf::Texture& texture_, const sf::Sprite& sprite_, const vec2f& position, sf::RenderWindow* window):
            BaseUIElement(window),
            texture(texture_),
            sprite(sprite_) 
        {
            sprite.setTexture(texture);
            set_position(position);
        }

        void set_new_image(const sf::Texture& texture_, const sf::Sprite& sprite_) override{
            texture = texture_;
            sprite = sprite_;
        }
        
        void draw_() override{
            sprite.setPosition(position + relative);
            window_ptr->draw(sprite);
        }
    };

    class Button: public BaseUIElement{
        private:
        sf::Texture texture;
        sf::Sprite button_sprite;
        std::function<void()> func;
        sf::Mouse::Button trigger_button;

        public:
        Button():
            BaseUIElement{},
            texture{},
            button_sprite{},
            func{},
            trigger_button{sf::Mouse::Left}
        {}
        Button(
            const sf::Texture& texture_,
            const sf::Sprite& sprite, 
            const vec2f& position, 
            sf::RenderWindow* window, 
            void (*function_to_be_called)()=[]{},
            const sf::Mouse::Button& trigger=sf::Mouse::Left):

            BaseUIElement{window},
            texture{texture_},
            button_sprite{sprite},
            func{function_to_be_called},
            trigger_button{trigger}
        {
            button_sprite.setTexture(texture);
            set_position(position);
        }

        void set_function(std::function<void()> func_) override{
            func = func_;
        }

        void draw_() override{
            button_sprite.setPosition(position + relative);
            window_ptr->draw(button_sprite);
        }

        void update_(const was::MouseManager& mouse) override;
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
        UIScheme& operator =(const UIScheme& other); 
        UIScheme(sf::RenderWindow* window):
            element_ptrs{},
            window_ptr{window},
            scheme_src{}
        {}
        ~UIScheme();
        UIScheme(UIScheme&& other);
        UIScheme& operator=(UIScheme&& rhs);

        void load(const YAML::Node& node);

        void draw();

        void update(const was::MouseManager& mouse);

        BaseUIElement* get_ptr_by_name(const std::string& name);

        private:
        Text* load_text(const YAML::Node& node);
        Rectangle* load_rectangle(const YAML::Node& node);
        Button* load_button(const YAML::Node& node);
        Image* load_image(const YAML::Node& node);
    };
}



#endif