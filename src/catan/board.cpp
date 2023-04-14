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
    std::vector<std::string> building_types = ctn::house_types;
    building_types.insert(building_types.end(), ctn::path_types.begin(), ctn::path_types.end());
    sprites = ctn::load_sprites(assets_cfg["Buildings"], texture, building_types);

    house_id = new was::Text("", font, window, 14, sf::Color::Black, sf::Vector2f(0, 0));
}

void ctn::Board::generate_board(){
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

    for(ctn::Path& path : path_rend){
        window->draw(path.sprite);
    }
}

void ctn::Board::attribute_resources(const std::vector<ctn::BoardTile>& tiles){
    typedef std::pair<sf::Vector2f, ctn::Tile> Resource;

    int max_resources = config["max"].as<int>();
    int search_distance = config["search_distance"].as<int>();
    sf::Vector2f tiles_offset(config["tiles_center_offset"][0].as<int>(), config["tiles_center_offset"][1].as<int>());
    
    std::vector<Resource> resources;
    for(const ctn::BoardTile& bt : tiles){
        resources.push_back(std::make_pair(bt.get_position() + tiles_offset, bt.get_type()));
    }

    for(Place& place : places){
        std::sort(resources.begin(), resources.end(), [&place](const Resource& lhs, const Resource& rhs){
            return was::distance_sq(place.get_position(), lhs.first) < was::distance_sq(place.get_position(), rhs.first);
        });

        std::cout << "Resources for place id [" << place.get_id() << "]: ";
        for(int i = 0; i < max_resources; i++){
            if(was::distance_sq(place.get_position(), resources[i].first) > search_distance * search_distance) continue;

            place.add_resource(resources[i].second);
            // std::cout << ctn::enum2str.at(resources[i].second) << " ";
        }
        std::cout << std::endl;
    }
}


sf::Vector2f ctn::Board::is_connected(Place& pl1, Place& pl2, const std::vector<sf::Vector2f>& directions, int max_radius){
    for(const sf::Vector2f& dir : directions){
        if(was::distance_sq(pl1.get_position() + dir, pl2.get_position()) <= max_radius * max_radius){
            return dir;
        }      
    }

    return sf::Vector2f(0, 0);
}

std::string ctn::Board::get_path_type(sf::Vector2f dir){
    std::vector<sf::Vector2f> dirs;
    std::vector<std::string> dirs_type;
    
    for(std::string path_type : ctn::path_types){
        int x = config["dir_names"][path_type][0].as<int>();
        int y = config["dir_names"][path_type][1].as<int>();
        
        std::cout << path_type << " " << x << ' ' << y << std::endl;

        dirs.push_back(sf::Vector2f(x, y));
        dirs_type.push_back(path_type);
        dirs.push_back(sf::Vector2f(-x, -y));
        dirs_type.push_back(path_type);
    }

    for(int i = 0; i < dirs.size(); i++){
        if(was::distance_sq(dir, dirs[i]) < 5){
            return dirs_type[i];
        }
    }
    std::cout << "WHat?" << std::endl;
    return "Nope";
}

void ctn::Board::make_path_if_exist(Place& pl1, Place& pl2, const std::vector<sf::Vector2f>& directions){
    std::map<std::string, sf::Vector2f> path_position_offset;
    for(std::string path_type : ctn::path_types){
        std::cout << path_type << std::endl;
        path_position_offset[path_type] = sf::Vector2f(
                config["path_position_offset"][path_type][0].as<int>(),
                config["path_position_offset"][path_type][1].as<int>()
                );
    }


    sf::Vector2f dir = is_connected(pl1, pl2, directions);
    if(dir != sf::Vector2f(0, 0)){
        int done_path_id = -1;
        for(const PathData& pd : graph[pl2.get_id()]){
            if(pd.to == pl1.get_id()){
                done_path_id = pd.path_id;
                break;
            }
        }


        if(done_path_id == -1){
            std::string path_type = get_path_type(dir);
            // std::cout << path_type << std::endl;
            sprites[path_type].setPosition(sf::Vector2f(std::min(
                            pl1.get_position().x,
                            pl2.get_position().x),
                                        std::min(
                                            pl1.get_position().y, 
                                            pl2.get_position().y))
                            + path_position_offset[path_type]);
            path_rend.push_back(Path(sprites[path_type], path_type));
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), path_rend.size() - 1));
        }else{
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), done_path_id));
        }
    }
}

void ctn::Board::generate_graph(){
    graph = std::vector<std::vector<PathData>>(places.size(), std::vector<PathData>(0));
    std::vector<sf::Vector2f> search_directions;

    for(YAML::Node nd : config["search_directions"]){
        int x, y;
        x = nd[0].as<int>();
        y = nd[1].as<int>();
        search_directions.push_back(sf::Vector2f(x, y));
        search_directions.push_back(sf::Vector2f(-x, -y));
    }

    for(Place& place : places){
        for(Place& target : places){
            make_path_if_exist(place, target, search_directions);
        }
    }

    
    for(int i = 0; i < graph.size(); i++){
        // std::cout << "House " << i << " is connected to: ";        
        for(PathData pd : graph[i]){
           // std::cout << pd.to << " ";
        }

        // std::cout << std::endl;
    }
}

#pragma endregion
