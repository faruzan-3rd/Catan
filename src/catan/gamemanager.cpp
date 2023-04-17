#include "catan/gamemanager.hpp"

ctn::Message message = {"init", -1, "", "", "", ""};
std::vector<ctn::Player> players;
std::optional<was::EventManager> eventmgr;


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
    board.generate_harbors();

    players.push_back(Player(ctn::RED));
    players.push_back(Player(ctn::BLUE));
    players.push_back(Player(ctn::GREEN));

    eventmgr.emplace(window);
}


bool ctn::GameManager::tick(){
    
    if(!eventmgr->tick()){
        return 1;
    }
    turnmng.tick();
    progmng.tick(&board);


    return 0;
}

void ctn::GameManager::draw(){
    tile_renderer.draw();
    board.draw();
}
