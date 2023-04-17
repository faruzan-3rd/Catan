#ifndef __CTN__GMANAGER_HPP
#define __CTN__GMANAGER_HPP

#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "catan/board.hpp"
#include "catan/tiles.hpp"
#include "catan/message.hpp"
#include "catan/player.hpp"
#include "catan/turn.hpp"
#include "catan/progress.hpp"
#include "wastils/events.hpp"
#include <optional>


namespace ctn{
    class GameManager{
        sf::RenderWindow* window;

        YAML::Node config;
        ctn::TileRenderer tile_renderer;
        ctn::Board board;
        ctn::TurnManager turnmng;
        ctn::ProgressManager progmng;

        public:
        GameManager();
        GameManager(YAML::Node config_, sf::RenderWindow* window_);

        bool tick();

        void draw();
    };
}


#endif