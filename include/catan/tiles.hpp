#ifndef __CATAN_TILES_HPP
#define __CATAN_TILES_HPP

#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"
#include "catan/constants.hpp"
#include "catan/assets.hpp"
#include "wastils/operator.hpp"
#include <vector>
#include <random>


namespace ctn {

    class BoardTile{
        sf::Sprite sprite;
        ctn::Tile tile_type;
        sf::RenderWindow* window;
        sf::Vector2f coordinates;

        public:
        BoardTile();
        BoardTile(sf::Sprite sprite_, ctn::Tile tile_type_, sf::RenderWindow* window_, sf::Vector2f coordinates_);

        void draw();

        sf::Vector2f get_position() const;

        ctn::Tile get_type() const;
    };


    class TileRenderer{
        YAML::Node general_config, tiles_config, env_config;
        sf::RenderWindow* window_ptr;
        
        sf::Texture texture, env_texture;
        std::map<ctn::Tile, sf::Sprite> tiles;
        std::vector<sf::Sprite> sea_tiles;
        int sea_width, sea_height;

        sf::Vector2f tile_offset;

        // vectors that define tile coordinates
        sf::Vector2f u, v;

        std::vector<sf::Sprite> sea_rend;
        std::vector<ctn::BoardTile> tiles_rend;


        public:
        TileRenderer();
        
        void load_assets(const YAML::Node& config_, sf::RenderWindow* window_ptr_);

        void generate_tiles();

        std::vector<ctn::BoardTile> get_tiles() const;

        void draw();
    };
}


#endif