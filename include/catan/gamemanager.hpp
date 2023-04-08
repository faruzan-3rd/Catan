#ifndef __CTN__GMANAGER_HPP
#define __CTN__GMANAGER_HPP

#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "catan/board.hpp"
#include "catan/tiles.hpp"


namespace ctn
{
    class GameManager{
        sf::RenderWindow* window;

        YAML::Node config;
        ctn::TileRenderer tile_renderer;
        ctn::Board board;

        public:
        GameManager();
        GameManager(YAML::Node config_, sf::RenderWindow* window_);

        void tick();

        void draw();
    };
} 


#endif