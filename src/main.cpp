#include "SFML/Graphics.hpp"
#include "wastils/config.hpp"
#include <iostream>


int main(){
    YAML::Node config;
    if(was::load_config(config, "assets/config.yaml") == 1){
        return 1;
    }
    
    int sc_w = config["Window"]["width"].as<int>(), sc_h = config["Window"]["height"].as<int>();
    std::string title = config["Title"].as<std::string>();
    sf::RenderWindow window(sf::VideoMode(sc_w, sc_h), title, sf::Style::Titlebar | sf::Style::Close);

    std::cout << "Window generated" << std::endl;

    window.setFramerateLimit(config["Game"]["framerate"].as<int>());
    window.setKeyRepeatEnabled(false);
    sf::Color backgroundColor(config["Background"]["r"].as<int>(), config["Background"]["g"].as<int>(), config["Background"]["b"].as<int>());

    std::cout << "Loading finished" << std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed){
                window.close();
                return 0;
            }
        }
        window.clear(backgroundColor);
        window.display();
    }  
}