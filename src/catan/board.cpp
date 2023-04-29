#include "catan/board.hpp"


bool ctn::Place::is_clicked(const vec2f& mouse_pos) const{
    return 
        (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + 30) &&
        (pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + 30);
}


bool ctn::Path::is_clicked(const vec2f& mouse_pos) const{
    return 
        (pos.x <= mouse_pos.x && mouse_pos.x <= pos.x + 30) &&
        (pos.y <= mouse_pos.y && mouse_pos.y <= pos.y + 30);
}


ctn::Harbor::Harbor(
            const std::string& req_mat_, 
            const int& req_num_, 
            const vec2f& mat_offset_, 
            const vec2f& pl1, 
            const vec2f& pl2,
            const vec2f& position_){
    std::complex<double> 
        pl1_pos(pl1.x, pl1.y),
        pl2_pos(pl2.x, pl2.y);
    
    rotation = std::arg(pl2_pos - pl1_pos) * ctn::rad2deg;
    position = position_;
    mat_position = position + mat_offset_;

    required_mat = req_mat_;
    required_num = req_num_;
}


void ctn::Board::generate_board(const YAML::Node& config_){
    if(was::load_config(config, config_["Game"]["board"].as<std::string>())){
        std::cout << "Error while loading board config (board)." << std::endl;
        return;
    }

    vec2f 
        u(config["u"][0].as<int>(), config["u"][1].as<int>()),
        v(config["v"][0].as<int>(), config["v"][1].as<int>()),
        place_offset(config["offset"][0].as<int>(), config["offset"][1].as<int>()),
        short_path(-config["short"].as<int>(), 0),
        long_path(-config["long"].as<int>(), 0);

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
            ctn::Place place(row_start + horizontal_offset, {}, places.size(), ctn::INVISIBLE);

            places.push_back(place);
            horizontal_offset += interval[(i + start_with_short) % 2];
        }
    }
}

void ctn::Board::generate_harbors(){
    std::vector<std::vector<int>> harbor_configs;

    int i = 0;
    for(YAML::Node node : config["harbors"]){
        std::vector<int> cfg = node.as<std::vector<int>>();
        if(cfg.size() != 4){
            std::cout << "Bad harbor format" << std::endl;
            i++;
            continue;
        }

        int pl1_id = cfg[0], pl2_id = cfg[1], posx = cfg[2], posy = cfg[3];
        vec2f 
            pl1 = places[pl1_id].get_position(),
            pl2 = places[pl2_id].get_position();

        int harbor_id = harbors.size();

        std::string mat = config["harbor-mats"][i].as<std::string>();
        int mat_num = config["harbor-mats-num"][i].as<int>();
        vec2f offset(
            config["harbor-mats-offset"][i][0].as<int>(),
            config["harbor-mats-offset"][i][1].as<int>()
        );

        harbors.push_back(Harbor(mat, mat_num, offset, pl1, pl2, vec2f(posx, posy)));
        places[pl1_id].set_harbor(harbor_id);
        places[pl2_id].set_harbor(harbor_id);
        i++;
    }
}


void ctn::Board::attribute_resources(){
    typedef std::pair<vec2f, std::string> Resource;

    int max_resources = config["max"].as<int>();
    int search_distance = config["search_distance"].as<int>();
    vec2f tiles_offset(config["tiles_center_offset"][0].as<int>(), config["tiles_center_offset"][1].as<int>());
    
    std::vector<Resource> resources;
    for(const ctn::BoardTile& bt : tiles){
        resources.push_back(std::make_pair(bt.get_position() + tiles_offset, bt.get_tile_type()));
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

const str ctn::Board::get_path_type(const vec2f& dir) const{
    std::vector<vec2f> dirs;
    std::vector<std::string> dirs_type;
    str ret{"Nope"};
    
    for(std::string path_type : ctn::path_types){
        int x = config["dir_names"][path_type][0].as<int>();
        int y = config["dir_names"][path_type][1].as<int>();

        dirs.push_back(vec2f(x, y));
        dirs_type.push_back(path_type);
        dirs.push_back(vec2f(-x, -y));
        dirs_type.push_back(path_type);
    }

    for(int i = 0; i < (int)dirs.size(); i++){
        if(was::distance_sq(dir, dirs[i]) < 5){
            ret = dirs_type[i];
            break;
        }
    }

    return ret;
}

void ctn::Board::make_path_if_exist(Place& pl1, Place& pl2, const std::vector<vec2f>& directions){
    std::map<std::string, vec2f> path_position_offset;
    for(std::string path_type : ctn::path_types){
        vec2f offset(
            config["path_position_offset"][path_type][0].as<int>(),
            config["path_position_offset"][path_type][1].as<int>());
        path_position_offset[path_type] = offset;
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
            float 
                minx = std::min(pl1.get_position().x, pl2.get_position().x),
                miny = std::min(pl1.get_position().y, pl2.get_position().y);

            vec2f pos = vec2f(minx, miny) + path_position_offset[path_type];
            paths.push_back(Path(path_type, pos, ctn::INVISIBLE, pl1.get_id(), pl2.get_id()));
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), paths.size() - 1));
        }else{
            graph[pl1.get_id()].push_back(PathData(pl2.get_id(), done_path_id));
        }
    }
}

void ctn::Board::generate_graph(){
    graph = std::vector<std::vector<PathData>>(places.size(), std::vector<PathData>(0));
    std::vector<vec2f> search_directions;

    for(YAML::Node nd : config["search_directions"]){
        int
            x = nd[0].as<int>(),
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

void ctn::Board::generate_tiles(const YAML::Node& config_){
    YAML::Node board_cfg, assets_cfg;
    if(was::load_config(board_cfg, config_["Game"]["board"].as<str>())){
        std::cout << "Error while loading board config (tiles)." << std::endl;
        return;
    }
    if(was::load_config(assets_cfg, config_["Game"]["assets"].as<str>())){
        std::cout << "Error while generating assets config (tiles)." << std::endl;
        return;
    }
    vec2f 
        u(
            assets_cfg["Tiles"]["u"][0].as<int>(), 
            assets_cfg["Tiles"]["u"][1].as<int>()
        ),
        v(
            assets_cfg["Tiles"]["v"][0].as<int>(), 
            assets_cfg["Tiles"]["v"][1].as<int>()
        );

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::vector<std::string> tiles_shuffle;
    for(std::string tile_type : tile_types){
        int num = board_cfg["tiles_num"][tile_type].as<int>();
        for(int _ = 0; _ < num; _++) tiles_shuffle.push_back(tile_type);
    }
    std::shuffle(tiles_shuffle.begin(), tiles_shuffle.end(), engine);

    vec2f tile_offset(
        assets_cfg["Tiles"]["offset"][0].as<int>(),
        assets_cfg["Tiles"]["offset"][1].as<int>()
    );
    vec2f tile_position;

    int tile_shape[] = {3, 4, 5, 4, 3};
    int start_index[] = {5, 2, 0, 1, 3};
    int middle_row_index[] = {0, 4, 9, 14, 18};
    const int col_tot = 5;
    tiles = std::vector<ctn::BoardTile>(19);

    int remain = col_tot;
    for(int shape : tile_shape){
        tile_position = tile_offset - u * (remain - 3) - v * std::min(2, col_tot - remain);

        for(int row = 0; row < shape; row++){
            int index = (shape == 5 ? middle_row_index[row] : (start_index[col_tot - remain] + 5 * row));
            std::string tile_typ = tiles_shuffle[index];

            ctn::BoardTile tile(tile_typ, tile_position);

            tiles[index] = tile;
            tile_position += v;
        }

        remain--;
    }

    std::reverse(tiles.begin(), tiles.end());


    int placement_index = 0;
    std::vector<int> 
        tokens = board_cfg["tokens"].as<std::vector<int>>(),
        token_placement = board_cfg["token_placement"].as<std::vector<int>>();

    for(int token : tokens){
        int tile_id = token_placement[placement_index];
        while(placement_index < (int)tiles.size() && tiles[tile_id].get_tile_type() == ctn::DESERT){
            placement_index++;
            tile_id = token_placement[placement_index];
        }
        tiles[tile_id].set_token(token);
        placement_index++;
    }
}

int ctn::Board::get_clicked_place(const vec2f& mouse_pos) const{
    for(int i = 0; i < (int)places.size(); i++){
        if(places[i].is_clicked(mouse_pos)){
            return places[i].get_id();
        }
    }
    return -1;
}

int ctn::Board::get_clicked_path(const vec2f& mouse_pos) const{
    for(int i = 0; i < (int)paths.size(); i++){
        if(paths[i].is_clicked(mouse_pos)){
            return i;
        }
    }
    return -1;
}

void ctn::Board::build_settlement(int id, const str& type_, const str& color){
    places[id].set_settlement(type_, color);
}

void ctn::Board::build_path(int id, const str& color){
    paths[id].set_path(color);
}
