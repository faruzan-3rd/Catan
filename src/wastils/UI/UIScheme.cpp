#include "wastils/UI.hpp"
#include "wastils/config.hpp"


was::UIScheme::UIScheme(){
    window_ptr = nullptr;
}

was::UIScheme::UIScheme(const std::string& scheme_location, sf::RenderWindow* window){
    window_ptr = window;
    if(was::load_config(scheme_src, scheme_location) == 1){
        std::cout << "Problem occured while loading UIs" << std::endl;
        return;
    }

    load(scheme_src);
}

was::UIScheme::~UIScheme(){
    for(auto p : element_ptrs){
        delete p.second;
    }
    element_ptrs.clear();
}

was::UIScheme::UIScheme(const UIScheme& scheme){
    window_ptr = scheme.window_ptr;
    scheme_src = scheme.scheme_src;

    load(scheme_src);
}

was::UIScheme& was::UIScheme::operator =(const UIScheme& other){
    window_ptr = other.window_ptr;
    scheme_src = other.scheme_src;

    load(scheme_src);

    return *this;
}

was::UIScheme::UIScheme(UIScheme&& other){
    element_ptrs = other.element_ptrs;
    window_ptr = other.window_ptr;
    scheme_src = other.scheme_src;

    other.element_ptrs.clear();
}

was::UIScheme& was::UIScheme::operator=(UIScheme&& rhs){
    if(this != &rhs){
        window_ptr = rhs.window_ptr;
        scheme_src = rhs.scheme_src;

        for(auto p : element_ptrs){
            delete p.second;
        }
        element_ptrs.clear();

        element_ptrs = rhs.element_ptrs;
        rhs.element_ptrs.clear();
    }

    return *this;
}


// ===========


void was::UIScheme::draw(){
    for(auto p : element_ptrs){
        p.second->draw();
    }
}

was::BaseUIElement* was::UIScheme::get_ptr_by_name(const std::string& name){
    return element_ptrs[name];
}

void was::UIScheme::load(const YAML::Node& node){
    for(auto elm : node["scheme"]){
        was::BaseUIElement* elm_ptr = nullptr;
        std::string elm_type = elm["type"].as<std::string>();
        std::string name = elm["name"].as<std::string>();

        if(elm_type == "text"){
            elm_ptr = load_text(elm);
        }
        else if(elm_type == "rectangle"){
            elm_ptr = load_rectangle(elm);
        }
        else if(elm_type == "button"){
            elm_ptr = load_button(elm);
        }

        if(elm_ptr != nullptr){
            element_ptrs[name] = elm_ptr;
        }
    }
}

void was::UIScheme::update(const was::MouseManager& mouse){
    for(auto pair : element_ptrs){
        pair.second->update_(mouse);
    }
}

was::Text* was::UIScheme::load_text(const YAML::Node& node){
    std::string txt = "";
    sf::Font font;
    int size = 15;
    sf::Color color(0, 0, 0);
    sf::Vector2f position(0, 0);

    txt = node["text"].as<std::string>();
    font.loadFromFile(node["font"].as<std::string>());
    size = node["size"].as<int>();
    color = sf::Color(node["color"][0].as<int>(), node["color"][1].as<int>(), node["color"][2].as<int>());
    position = sf::Vector2f(node["position"][0].as<int>(), node["position"][1].as<int>());

    was::Text* text_ptr = new was::Text(txt, font, window_ptr, size, color, position);

    return text_ptr;
}

was::Rectangle* was::UIScheme::load_rectangle(const YAML::Node& node){
    sf::IntRect rect_shape;
    int thickness;
    sf::Color color;

    rect_shape = sf::IntRect(sf::Vector2i(node["position"][0].as<int>(), node["position"][1].as<int>()),
                            sf::Vector2i(node["width"].as<int>(), node["height"].as<int>()));
    thickness = node["thickness"].as<int>();
    color = sf::Color(node["color"][0].as<int>(), node["color"][1].as<int>(), node["color"][2].as<int>());

    was::Rectangle* rect = new Rectangle(rect_shape, color, thickness, window_ptr);

    return rect;
}

was::Button* was::UIScheme::load_button(const YAML::Node& node){
    vec2f position(
        node["position"][0].as<int>(),
        node["position"][1].as<int>()
    );
    sf::IntRect rect(
        node["sprite"][0].as<int>(),
        node["sprite"][1].as<int>(),
        node["sprite"][2].as<int>(),
        node["sprite"][3].as<int>()
    );
    sf::Texture texture;
    texture.loadFromFile(node["texture"].as<std::string>());
    sf::Sprite mouse_sprite;
    mouse_sprite.setTextureRect(rect);

    return new Button(
        texture, 
        mouse_sprite,
        position,
        window_ptr
    );    
}
