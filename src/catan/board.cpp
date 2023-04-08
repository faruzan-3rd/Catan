#include "catan/board.hpp"


ctn::Place::Place(){}

ctn::Place::Place(sf::RenderWindow* window_, sf::Sprite sprite_, sf::Vector2f coords, std::vector<int> connected_to_){
    window = window_;
    sprite = sprite_;
    coordinates_on_screen = coords;
    sprite.setPosition(coordinates_on_screen);
    connected_to = connected_to_;
}

void ctn::Place::draw(){
    window->draw(sprite);
}


ctn::Board::Board(){

}

void ctn::Board::load_assets(YAML::Node config_, sf::RenderWindow* window_){
    was::load_config(config, config_["Game"]["board"].as<std::string>());
    was::load_config(assets_cfg, config_["Game"]["assets"].as<std::string>());
    assets_cfg = assets_cfg["Board"];
    window = window_;

    texture.loadFromFile(assets_cfg["texture"].as<std::string>());
    sprites = ctn::load_sprites(assets_cfg["Buildings"], texture, ctn::building_types);

}

void ctn::Board::generate_graph(){
    sf::Vector2f u(config["u"][0].as<int>(), config["u"][1].as<int>());
    sf::Vector2f v(config["v"][0].as<int>(), config["v"][1].as<int>());
    sf::Vector2f place_offset(config["offset"][0].as<int>(), config["offset"][1].as<int>());
    sf::Vector2f short_path(-config["short"].as<int>(), 0);
    sf::Vector2f long_path(-config["long"].as<int>(), 0);

    for(auto shape_node : config["shape"]){
        std::vector<int> shape = shape_node.as<std::vector<int>>();

        if(shape.size() != 4){
            std::cout << "Bad board shape format." << std::endl;
            return;
        }

        int num = shape[0], x = shape[1], y = shape[2];
        bool start_with_short = shape[3];
        sf::Vector2f row_start = place_offset + u * x + v * y;

        sf::Vector2f interval[2] = {long_path, short_path};
    
        sf::Vector2f horizontal_offset(0, 0);
        for(int i = 0; i < num; i++){
            ctn::Place place(window, sprites[ctn::HOUSE], row_start + horizontal_offset, {});

            places.push_back(place);
            horizontal_offset += interval[(i + start_with_short) % 2];
        }
    }
}

void ctn::Board::draw(){
    for(ctn::Place& p : places){
        p.draw();
    }
}