#include "catan/tiles.hpp"
#include "wastils/operator.hpp"


ctn::TileRenderer::TileRenderer(){

}

ctn::TileRenderer::TileRenderer(const YAML::Node& config_, sf::RenderWindow* window_ptr_){
    window_ptr = window_ptr_;
    config = config_["Tiles"];
    texture.loadFromFile(config["texture"].as<std::string>());
    env_texture.loadFromFile(config["Environment"]["texture"].as<std::string>());
    tiles = load_sprites(config, texture, config["scale"].as<float>());

    tile_offset = sf::Vector2f(config["offset"][0].as<int>(), config["offset"][1].as<int>());
    u = sf::Vector2f(config["u"][0].as<int>(), config["u"][1].as<int>());
    v = sf::Vector2f(config["v"][0].as<int>(), config["v"][1].as<int>());

    sea_tiles = ctn::load_from_list(config["Environment"]["Sea"]["Sprites"], env_texture, config["Environment"]["Sea"]["scale"].as<int>());
    sea_width = config["Environment"]["Sea"]["size"][0].as<int>();  
    sea_height = config["Environment"]["Sea"]["size"][1].as<int>();
}

void ctn::TileRenderer::generate_tiles(){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::uniform_int_distribution rand(0, (int)(sizeof(All_types)/sizeof(ctn::Tile) - 1));

    sf::Vector2f tile_position;

    int tile_shape[] = {3, 4, 5, 4, 3};
    int start_index[] = {5, 2, 0, 1, 3};
    int middle_row_index[] = {0, 4, 9, 14, 18};
    const int col_tot = 5;
    tiles_rend = std::vector<sf::Sprite>(19);

    int remain = col_tot;
    for(int shape : tile_shape){
        tile_position = tile_offset - u * (remain - 3) - v * std::min(2, col_tot - remain);

        for(int row = 0; row < shape; row++){
            ctn::Tile tile_typ = All_types[rand(engine)];

            sf::Sprite tile = tiles[tile_typ];
            tile.setPosition(tile_position);
            tiles_rend[(shape == 5 ? middle_row_index[row] : (start_index[col_tot - remain] + 5 * row))] = tile;
            tile_position += v;
        }

        remain--;
    }

    std::reverse(tiles_rend.begin(), tiles_rend.end());

    // Sea
    std::uniform_int_distribution rand_sea(0, (int)sea_tiles.size() - 1);
    for(int x = 0; x < sea_width; x++) for(int y = 0; y < sea_height; y++){
        sf::Sprite sea_tile = sea_tiles[rand_sea(engine)];
        sea_tile.setPosition(x * sea_tile.getTextureRect().width, y * sea_tile.getTextureRect().height);
        sea_rend.push_back(sea_tile);
    }
}

void ctn::TileRenderer::draw(){
    for(sf::Sprite& sp : sea_rend){
        window_ptr->draw(sp);
    }
    for(sf::Sprite& sp : tiles_rend){
        window_ptr->draw(sp);
    }
}