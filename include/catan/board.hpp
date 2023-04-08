#ifndef __CTN__BOARD_HPP
#define __CTN__BOARD_HPP

#include <map>
#include <vector>
#include "wastils/config.hpp"
#include "wastils/operator.hpp"
#include "catan/assets.hpp"
#include "catan/constants.hpp"
#include "SFML/Graphics.hpp"
#include "yaml-cpp/yaml.h"


namespace ctn{
    class Place{
        sf::RenderWindow* window;
        sf::Vector2f coordinates_on_screen;
        std::vector<int> connected_to;

        sf::Sprite sprite;

        public:
        Place();
        Place(sf::RenderWindow* window_, sf::Sprite sprite, sf::Vector2f coords, std::vector<int> connected_to_);

        void draw();
    };
    
    class Board{
        sf::RenderWindow* window;
        std::map<std::string, sf::Sprite> sprites;
        sf::Vector2f u, v;
        sf::Texture texture;
        YAML::Node config, assets_cfg;

        std::vector<ctn::Place> places;

        public:
        Board();

        void load_assets(YAML::Node config_, sf::RenderWindow* window_);

        void generate_graph();

        void draw();
    };
}


#endif