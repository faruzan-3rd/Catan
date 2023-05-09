#ifndef __CTN__GMANAGER_HPP
#define __CTN__GMANAGER_HPP

#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "catan/board.hpp"
#include "catan/player.hpp"
#include "catan/progress.hpp"
#include "catan/events.hpp"
#include "catan/graphics.hpp"
#include "wastils/input.hpp"
#include "wastils/UI.hpp"
#include <optional>
#include <map>
#include <vector>
#include <memory>

using str = std::string;
using sprt = sf::Sprite;
using vec2f = sf::Vector2f;
using vec2i = sf::Vector2i;


namespace ctn{

    class Audio{

    };

    class GameManager{
        sf::RenderWindow* window;

        YAML::Node config;
        was::MouseManager mouse;
        ctn::Board board;
        ctn::Graphics graphics;
        ctn::ProgressManager progressmng;
        ctn::EventManager eventmanager;
        ctn::PlayersManager playersmng;

        public:
        GameManager() = default;
        GameManager(YAML::Node config_, sf::RenderWindow* window_);

        bool tick();

        void draw();

        private:
        void attribute_functions_to_ui();
    };
}


#endif