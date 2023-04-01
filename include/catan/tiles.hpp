#ifndef __CATAN_TILES_HPP
#define __CATAN_TILES_HPP

#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"
#include "catan/constants.hpp"
#include "catan/assets.hpp"
#include <vector>
#include <random>


namespace ctn {

    class TileRenderer{
        YAML::Node config;
        sf::RenderWindow* window_ptr;
        
        sf::Texture texture;
        std::map<ctn::Tile, sf::Sprite> tiles;

        sf::Vector2f tile_offset;

        // vectors that define tile coordinates
        sf::Vector2f u, v;

        std::vector<sf::Sprite> tiles_render;


        public:
        TileRenderer();

        TileRenderer(const YAML::Node& config_, sf::RenderWindow* window_ptr_);

        void generate_tiles();

        void draw();
    };
}


#endif