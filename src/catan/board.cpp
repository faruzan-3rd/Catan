#include "catan/board.hpp"


#pragma region Place
ctn::Place::Place(){}

ctn::Place::Place(sf::RenderWindow* window_, sf::Sprite sprite_, vec2f coords, std::vector<int> connected_to_, int id_){
    window = window_;
    sprite = sprite_;
    pos = coords;
    sprite.setPosition(pos);
    connected_to = connected_to_;
    id = id_;
}

void ctn::Place::draw(){
    window->draw(sprite);
}

int ctn::Place::get_id() const{
    return id;
}

vec2f ctn::Place::get_position() const{
    return pos;
}

void ctn::Place::add_resource(ctn::Tile resource){
    available_resources.push_back(resource);
}

void ctn::Place::set_port(int port_id_){
    port_id = port_id_;
}

void ctn::Place::set_sprite(sf::Sprite sprite_){
    sprite = sprite_;
    sprite.setPosition(pos);
}

bool ctn::Place::is_clicked(const vec2f& mouse_pos){
    return 
    (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + sprite.getTextureRect().width) &&
    (pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + sprite.getTextureRect().height);
}

#pragma endregion


void ctn::Path::set_sprite(sf::Sprite sprite_){
    sprite_.setPosition(sprite.getPosition());
    sprite = sprite_;
}

bool ctn::Path::is_clicked(const vec2f& mouse_pos){
    vec2f pos = sprite.getPosition();

    return (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + sprite.getTextureRect().width) &&
    (pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + sprite.getTextureRect().height);
}



ctn::Port::Port(){}

ctn::Port::Port(sf::Sprite sprite_, sf::Sprite resource_sprite_, sf::RenderWindow* window_, std::string req_mat_, int req_num_, vec2f mat_offset_, vec2f pl1, vec2f pl2){
    window = window_;
    sprite = sprite_;
    resource_sprite = resource_sprite_;
    std::complex<double> 
        pl1_pos(pl1.x, pl1.y),
        pl2_pos(pl2.x, pl2.y);
    
    double angle = std::arg(pl2_pos - pl1_pos);
    sprite.rotate(angle * ctn::rad2deg);
    resource_sprite.setPosition(sprite.getPosition() + mat_offset_);

    required_mat = req_mat_;
    required_num = req_num_;
}

void ctn::Port::draw(){
    window->draw(sprite);
    window->draw(resource_sprite);
}



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

    window = window_;

    texture.loadFromFile(assets_cfg["Board"]["texture"].as<std::string>());
    env_texture.loadFromFile(assets_cfg["Environment"]["texture"].as<std::string>());
    
    assets_cfg = assets_cfg["Board"];
    std::vector<std::string> building_types = ctn::house_types + ctn::path_types;

    std::map<std::string, sf::Sprite> tmp;
    for(std::string color : colors){
        int offset_x, offset_y;
        offset_x = assets_cfg["Buildings"]["Color-offsets"][color][0].as<int>();
        offset_y = assets_cfg["Buildings"]["Color-offsets"][color][1].as<int>();

        tmp = ctn::load_sprites(assets_cfg["Buildings"], texture, building_types, 1.0f, 255, offset_x, offset_y, color);
        for(auto p : tmp){
            sprites[p.first] = p.second;
        }
    }
    sprites[ctn::NONE] = load_sprite(assets_cfg["Buildings"]["Sprites"]["none"], texture);

    std::vector<std::string> mats = ctn::materials;
    mats.push_back(ctn::mANY);
    port_mat_sprites = ctn::load_sprites(assets_cfg["Port"], texture, mats);

    house_id = new was::Text("", font, window, 14, sf::Color::Black, vec2f(0, 0));
}

void ctn::Board::generate_board(){
    vec2f u(config["u"][0].as<int>(), config["u"][1].as<int>());
    vec2f v(config["v"][0].as<int>(), config["v"][1].as<int>());
    vec2f place_offset(config["offset"][0].as<int>(), config["offset"][1].as<int>());
    vec2f short_path(-config["short"].as<int>(), 0);
    vec2f long_path(-config["long"].as<int>(), 0);

    for(auto shape_node : config["shape"]){
        std::vector<int> shape = shape_node.as<std::vector<int>>();

        if(shape.size() != 4){
            std::cout << "Bad board shape format." << std::endl;
            return;
        }

        int num = shape[0], x = shape[1], y = shape[2];
        bool start_with_short = shape[3];
        vec2f row_start = place_offset + u * x + v * y;

        vec2f interval[2] = {long_path, short_path};
    
        vec2f horizontal_offset(0, 0);
        for(int i = 0; i < num; i++){
            ctn::Place place(window, sprites[ctn::NONE], row_start + horizontal_offset, {}, places.size());

            places.push_back(place);
            horizontal_offset += interval[(i + start_with_short) % 2];
        }
    }
}

void ctn::Board::generate_ports(){
    std::vector<std::vector<int>> port_configs;
    sf::Sprite port = ctn::load_sprite(config["port_sprite"], env_texture, 1.5f);

    int i = 0;
    for(YAML::Node node : config["ports"]){
        std::vector<int> cfg = node.as<std::vector<int>>();
        if(cfg.size() != 4){
            std::cout << "Bad port format" << std::endl;
            i++;
            continue;
        }

        int pl1_id = cfg[0], pl2_id = cfg[1], posx = cfg[2], posy = cfg[3];
        vec2f 
            pl1 = places[pl1_id].get_position(),
            pl2 = places[pl2_id].get_position();

        port.setPosition(posx, posy);
        int port_id = ports.size();

        std::string mat = config["port-mats"][i].as<std::string>();
        int mat_num = config["port-mats-num"][i].as<int>();
        vec2f offset(
            config["port-mats-offset"][i][0].as<int>(),
            config["port-mats-offset"][i][1].as<int>()
        );

        ports.push_back(Port(port, port_mat_sprites[mat], window, mat, mat_num, offset, pl1, pl2));
        places[pl1_id].set_port(port_id);
        places[pl2_id].set_port(port_id);
        i++;
    }
}

void ctn::Board::draw(){
    for(ctn::Port& port : ports){
        port.draw();
    }
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
    typedef std::pair<vec2f, ctn::Tile> Resource;

    int max_resources = config["max"].as<int>();
    int search_distance = config["search_distance"].as<int>();
    vec2f tiles_offset(config["tiles_center_offset"][0].as<int>(), config["tiles_center_offset"][1].as<int>());
    
    std::vector<Resource> resources;
    for(const ctn::BoardTile& bt : tiles){
        resources.push_back(std::make_pair(bt.get_position() + tiles_offset, bt.get_type()));
    }

    for(Place& place : places){
        std::sort(resources.begin(), resources.end(), [&place](const Resource& lhs, const Resource& rhs){
            return was::distance_sq(place.get_position(), lhs.first) < was::distance_sq(place.get_position(), rhs.first);
        });

        for(int i = 0; i < max_resources; i++){
            if(was::distance_sq(place.get_position(), resources[i].first) > search_distance * search_distance) continue;
            place.add_resource(resources[i].second);
        }
    }
}


vec2f ctn::Board::is_connected(Place& pl1, Place& pl2, const std::vector<vec2f>& directions, int max_radius){
    for(const vec2f& dir : directions){
        if(was::distance_sq(pl1.get_position() + dir, pl2.get_position()) <= max_radius * max_radius){
            return dir;
        }      
    }

    return vec2f(0, 0);
}

std::string ctn::Board::get_path_type(vec2f dir){
    std::vector<vec2f> dirs;
    std::vector<std::string> dirs_type;
    
    for(std::string path_type : ctn::path_types){
        int x = config["dir_names"][path_type][0].as<int>();
        int y = config["dir_names"][path_type][1].as<int>();

        dirs.push_back(vec2f(x, y));
        dirs_type.push_back(path_type);
        dirs.push_back(vec2f(-x, -y));
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

void ctn::Board::make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions){
    std::map<std::string, vec2f> path_position_offset;
    for(std::string path_type : ctn::path_types){
        path_position_offset[path_type] = vec2f(
                config["path_position_offset"][path_type][0].as<int>(),
                config["path_position_offset"][path_type][1].as<int>()
                );
    }


    vec2f dir = is_connected(pl1, pl2, directions);
    if(dir != vec2f(0, 0)){
        int done_path_id = -1;
        for(const PathData& pd : graph[pl2.get_id()]){
            if(pd.to == pl1.get_id()){
                done_path_id = pd.path_id;
                break;
            }
        }


        if(done_path_id == -1){
            std::string path_type = get_path_type(dir);
            std::string path_final_name = ctn::BLUE + path_type;
            sprites[ctn::NONE].setPosition(vec2f(std::min(
                            pl1.get_position().x,
                            pl2.get_position().x),
                                        std::min(
                                            pl1.get_position().y, 
                                            pl2.get_position().y))
                            + path_position_offset[path_type]);
            path_rend.push_back(Path(sprites[ctn::NONE], path_type));
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), path_rend.size() - 1));
        }else{
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), done_path_id));
        }
    }
}

void ctn::Board::generate_graph(){
    graph = std::vector<std::vector<PathData>>(places.size(), std::vector<PathData>(0));
    std::vector<vec2f> search_directions;

    for(YAML::Node nd : config["search_directions"]){
        int x, y;
        x = nd[0].as<int>();
        y = nd[1].as<int>();
        search_directions.push_back(vec2f(x, y));
        search_directions.push_back(vec2f(-x, -y));
    }

    for(Place& place : places){
        for(Place& target : places){
            make_path_if_exist(place, target, search_directions);
        }
    }
}


void ctn::Board::click(const vec2f& mouse_pos){
    for(Place& place : places){
        if(place.is_clicked(mouse_pos)){
            place.set_sprite(sprites[ctn::RED + ctn::HOUSE]);
        }
    }

    for(Path& path : path_rend){
        if(path.is_clicked(mouse_pos)){
            path.set_sprite(sprites[ctn::BLUE + path.path_typ]);
        }
    }
}

#pragma endregion
