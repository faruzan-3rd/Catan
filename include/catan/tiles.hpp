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

        std::vector<sf::Sprite> tiles_rend, sea_rend;


        public:
        TileRenderer();
        
        void load_assets(const YAML::Node& config_, sf::RenderWindow* window_ptr_);

        void generate_tiles();

        void draw();
    };
}


#endif