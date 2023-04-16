#ifndef __CATAN_TILES_HPP
#define __CATAN_TILES_HPP

#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"
#include "catan/constants.hpp"
#include "catan/assets.hpp"
#include "wastils/operator.hpp"
#include "wastils/UI.hpp"
#include <vector>
#include <random>

typedef sf::Vector2f vec2f;


namespace ctn {

    class BoardTile{
        sf::Sprite sprite;
        ctn::Tile tile_type;
        sf::RenderWindow* window;
        vec2f coordinates;
        int token;

        public:
        BoardTile();
        BoardTile(sf::Sprite sprite_, ctn::Tile tile_type_, sf::RenderWindow* window_, vec2f coordinates_);

        void draw();

        vec2f get_position() const;

        ctn::Tile get_type() const;

        int get_token() const;

        void set_token(int token_);
    };


    class TileRenderer{
        YAML::Node general_config, tiles_config, env_config, board_config;
        sf::RenderWindow* window_ptr;
        
        sf::Texture texture, env_texture;
        std::map<ctn::Tile, sf::Sprite> tiles;
        std::vector<sf::Sprite> sea_tiles;
        int sea_width, sea_height;

        vec2f tile_offset, token_offset, token_txt_offset;

        // vectors that define tile coordinates
        vec2f u, v;

        std::vector<sf::Sprite> sea_rend;
        std::vector<ctn::BoardTile> tiles_rend;
        sf::Texture token_img;
        sf::Sprite token_rend;
        was::Text* tile_token_txt = nullptr;
        sf::Font font;


        public:
        TileRenderer();
        ~TileRenderer();
        
        void load_assets(const YAML::Node& config_, sf::RenderWindow* window_ptr_);

        void generate_tiles();

        std::vector<ctn::BoardTile> get_tiles() const;

        void draw();
    };
}


#endif