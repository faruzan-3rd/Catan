#include "catan/gamemanager.hpp"


ctn::GameManager::GameManager(){

}


ctn::GameManager::GameManager(YAML::Node config_, sf::RenderWindow* window_){
    config = config_;
    window = window_;

    tile_renderer.load_assets(config, window);
    tile_renderer.generate_tiles();

    board.load_assets(config_, window);
    board.generate_board();
    board.attribute_resources(tile_renderer.get_tiles());
    board.generate_graph();
    board.generate_ports();
}


bool ctn::GameManager::tick(){
    sf::Event event;
    while (window->pollEvent(event))
    {
        if(event.type == sf::Event::Closed){
            window->close();
            return 1;
        }

        static bool lock_click;
        if (event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left && lock_click != true) {
                board.click((sf::Vector2f)sf::Mouse::getPosition(*window));
                lock_click = true; 
            }   
        }

        if (event.type == sf::Event::MouseButtonReleased){
            if (event.mouseButton.button == sf::Mouse::Left){
                lock_click = false; 
            }
        }

        
    }


    return 0;
}

void ctn::GameManager::draw(){
    tile_renderer.draw();
    board.draw();
}
