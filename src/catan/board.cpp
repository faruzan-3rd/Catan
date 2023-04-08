#include "catan/board.hpp"


#pragma region Place
ctn::Place::Place(){}

ctn::Place::Place(sf::RenderWindow* window_, sf::Sprite sprite_, sf::Vector2f coords, std::vector<int> connected_to_, int id_){
    window = window_;
    sprite = sprite_;
    coordinates_on_screen = coords;
    sprite.setPosition(coordinates_on_screen);
    connected_to = connected_to_;
    id = id_;
}

void ctn::Place::draw(){
    window->draw(sprite);
}

int ctn::Place::get_id() const{
    return id;
}

sf::Vector2f ctn::Place::get_position() const{
    return coordinates_on_screen;
}

void ctn::Place::add_resource(ctn::Tile resource){
    available_resources.push_back(resource);
}

#pragma endregion


#pragma region Board
ctn::Board::Board(){
}

ctn::Board::~Board(){
    delete house_id;
}

void ctn::Board::load_assets(YAML::Node config_, sf::RenderWindow* window_){
    was::load_config(config, config_["Game"]["board"].as<std::string>());
    was::load_config(assets_cfg, config_["Game"]["assets"].as<std::string>());
    font.loadFromFile(assets_cfg["Test"]["font"].as<std::string>());

    assets_cfg = assets_cfg["Board"];
    window = window_;

    texture.loadFromFile(assets_cfg["texture"].as<std::string>());
    sprites = ctn::load_sprites(assets_cfg["Buildings"], texture, ctn::building_types);

    house_id = new was::Text("", font, window, 14, sf::Color::Black, sf::Vector2f(0, 0));
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
            ctn::Place place(window, sprites[ctn::HOUSE], row_start + horizontal_offset, {}, places.size());

            places.push_back(place);
            horizontal_offset += interval[(i + start_with_short) % 2];
        }
    }
}

void ctn::Board::draw(){
    for(ctn::Place& p : places){
        p.draw();

        house_id->set_text(std::to_string(p.get_id()));
        house_id->set_position(p.get_position());
        house_id->draw();
    }
}

void ctn::Board::associate_resources(const std::vector<ctn::BoardTile>& tiles){
    typedef std::pair<sf::Vector2f, ctn::Tile> Ressource;

    int max_ressources = config["max"].as<int>();
    int search_distance = config["search_distance"].as<int>();
    sf::Vector2f tiles_offset(config["tiles_center_offset"][0].as<int>(), config["tiles_center_offset"][1].as<int>());
    
    std::vector<Ressource> ressources;
    for(const ctn::BoardTile& bt : tiles){
        ressources.push_back(std::make_pair(bt.get_position() + tiles_offset, bt.get_type()));
    }

    for(Place& place : places){
        std::sort(ressources.begin(), ressources.end(), [&place](const Ressource& lhs, const Ressource& rhs){
            return was::distance_sq(place.get_position(), lhs.first) < was::distance_sq(place.get_position(), rhs.first);
        });

        std::cout << "Ressources for place id [" << place.get_id() << "]: ";
        for(int i = 0; i < max_ressources; i++){
            if(was::distance_sq(place.get_position(), ressources[i].first) > search_distance * search_distance) continue;

            place.add_resource(ressources[i].second);
            std::cout << ctn::enum2str.at(ressources[i].second) << " ";
        }
        std::cout << std::endl;
    }
}

#pragma endregion