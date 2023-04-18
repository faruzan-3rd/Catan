#include "catan/gamemanager.hpp"

std::vector<ctn::Player> players;
std::optional<was::EventManager> eventmgr;


ctn::GameManager::GameManager(){

}


ctn::GameManager::GameManager(YAML::Node config_, sf::RenderWindow* window_){
    config = config_;
    window = window_;

    board.generate_board(config);
    board.generate_tiles(config);
    board.attribute_resources();
    board.generate_graph();
    board.generate_harbors();

    graphics.load_sprites(config, window);

    players.push_back(Player(ctn::RED));
    players.push_back(Player(ctn::BLUE));
    players.push_back(Player(ctn::GREEN));

    eventmgr.emplace(window);
}


bool ctn::GameManager::tick(){
    if(!eventmgr->tick()){
        return 1;
    }

    return 0;
}

void ctn::GameManager::draw(){
    graphics.draw(
        board.get_tiles(),
        board.get_places(),
        board.get_harbors(),
        board.get_paths());
}
