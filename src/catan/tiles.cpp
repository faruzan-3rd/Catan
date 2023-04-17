#include "catan/tiles.hpp"


void ctn::TileRenderer::load_assets(const YAML::Node& config_, sf::RenderWindow* window_ptr_){
    window_ptr = window_ptr_;
    std::cout << was::load_config(general_config, config_["Game"]["assets"].as<std::string>()) << std::endl;;
    tiles_config = general_config["Tiles"];
    env_config = general_config["Environment"];
    std::cout << was::load_config(board_config, config_["Game"]["board"].as<std::string>()) << std::endl;

    std::cout << "Tile texutre: " << texture.loadFromFile(tiles_config["texture"].as<std::string>()) << std::endl;
    tiles = load_sprites(tiles_config, texture, tile_types, tiles_config["scale"].as<float>());

    tile_offset = vec2f(tiles_config["offset"][0].as<int>(), tiles_config["offset"][1].as<int>());
    u = vec2f(tiles_config["u"][0].as<int>(), tiles_config["u"][1].as<int>());
    v = vec2f(tiles_config["v"][0].as<int>(), tiles_config["v"][1].as<int>());

    token_offset = vec2f(board_config["token_offset"][0].as<int>(), board_config["token_offset"][1].as<int>());
    token_txt_offset = vec2f(board_config["token_txt_offset"][0].as<int>(), board_config["token_txt_offset"][1].as<int>());

    env_texture.loadFromFile(env_config["texture"].as<std::string>());
    sea_tiles = ctn::load_from_list(env_config["Sea"]["Sprites"], env_texture, env_config["Sea"]["scale"].as<int>());
    sea_width = env_config["Sea"]["size"][0].as<int>();  
    sea_height = env_config["Sea"]["size"][1].as<int>();

    font.loadFromFile(general_config["Test"]["font"].as<std::string>());
    tile_token_txt = new was::Text("", font, window_ptr, 18, sf::Color::Black, vec2f(0, 0));
    token_img.loadFromFile(board_config["token_sprite"].as<std::string>());
    token_rend.setTexture(token_img);
}

void ctn::TileRenderer::generate_tiles(){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());

    std::vector<std::string> tiles_shuffle;
    for(std::string tile_type : tile_types){
        int num = board_config["tiles_num"][tile_type].as<int>();
        for(int _ = 0; _ < num; _++) tiles_shuffle.push_back(tile_type);
    }
    std::shuffle(tiles_shuffle.begin(), tiles_shuffle.end(), engine);

    vec2f tile_position;

    int tile_shape[] = {3, 4, 5, 4, 3};
    int start_index[] = {5, 2, 0, 1, 3};
    int middle_row_index[] = {0, 4, 9, 14, 18};
    const int col_tot = 5;
    tiles_rend = std::vector<ctn::BoardTile>(19);

    int remain = col_tot;
    for(int shape : tile_shape){
        tile_position = tile_offset - u * (remain - 3) - v * std::min(2, col_tot - remain);

        for(int row = 0; row < shape; row++){
            int index = (shape == 5 ? middle_row_index[row] : (start_index[col_tot - remain] + 5 * row));
            std::string tile_typ = tiles_shuffle[index];

            sf::Sprite tile_sp = tiles[tile_typ];
            ctn::BoardTile tile(window_ptr, tile_sp, tile_typ, tile_position);

            tiles_rend[index] = tile;
            tile_position += v;
        }

        remain--;
    }

    std::reverse(tiles_rend.begin(), tiles_rend.end());

    std::vector<int> tokens = board_config["tokens"].as<std::vector<int>>();
    std::vector<int> token_placement = board_config["token_placement"].as<std::vector<int>>();
    int placement_index = 0;
    for(int token : tokens){
        int tile_id = token_placement[placement_index];
        while(placement_index < tiles_rend.size() && tiles_rend[tile_id].get_tile_type() == ctn::DESERT){
            placement_index++;
            tile_id = token_placement[placement_index];
        }
        tiles_rend[tile_id].set_token(token);
        placement_index++;
    }

    // Sea
    std::uniform_int_distribution rand_sea(0, (int)sea_tiles.size() - 1);
    for(int x = 0; x < sea_width; x++) for(int y = 0; y < sea_height; y++){
        sf::Sprite sea_tile = sea_tiles[rand_sea(engine)];
        sea_tile.setPosition(x * sea_tile.getTextureRect().width, y * sea_tile.getTextureRect().height);
        sea_rend.push_back(sea_tile);
    }
}

std::vector<ctn::BoardTile> ctn::TileRenderer::get_tiles() const{
    return tiles_rend;
}

void ctn::TileRenderer::draw(){
    for(sf::Sprite& sp : sea_rend){
        window_ptr->draw(sp);
    }
    for(ctn::BoardTile& sp : tiles_rend){
        sp.draw();
        if(sp.get_token() <= 0) continue;
        
        token_rend.setPosition(sp.getPosition() + token_offset);
        window_ptr->draw(token_rend);

        tile_token_txt->set_position(sp.getPosition() + token_txt_offset);
        tile_token_txt->set_text(std::to_string(sp.get_token()));
        tile_token_txt->draw();
    }
}

#pragma endregion