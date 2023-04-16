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


void ctn::GameManager::tick(){


}

void ctn::GameManager::draw(){
    tile_renderer.draw();
    board.draw();
}
