#ifndef __CTN__GMANAGER_HPP
#define __CTN__GMANAGER_HPP

#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "catan/board.hpp"
#include "catan/player.hpp"
#include "wastils/events.hpp"
#include <optional>
#include <map>
#include <vector>
#include <memory>

typedef std::string str;
typedef sf::Sprite sprt;
typedef sf::Vector2f vec2f;
typedef sf::Vector2i vec2i;


namespace ctn{
    struct graphicscfg{
        vec2f
            tile_offset,
            token_offset,
            token_txt_offset;

        vec2i
            u,
            v,
            sea_dimension;
    };

    class Graphics{
        sf::RenderWindow* window;

        // Textures
        sf::Texture
            tx_board_objects,
            tx_environment,
            tx_tilemap;

        // In-game sprite map
        std::map<str, sprt>
            sp_tiles,
            sp_board_obj,
            sp_harbor;
        
        // Font
        sf::Font ft_mario;

        // UI
        std::unique_ptr<was::Text>
            txt_token;

        // Some config
        ctn::graphicscfg gracfg;
        

        public:
        Graphics():
            sp_tiles{},
            sp_board_obj{},
            sp_harbor{}
        {}

        void load_sprites(const YAML::Node& config_, sf::RenderWindow* window_);

        void draw(
            const std::vector<ctn::BoardTile>& tiles, 
            const std::vector<ctn::Place>& places,
            const std::vector<ctn::Harbor>& harbors,
            const std::vector<ctn::Path>& paths);
    };

    class Audio{

    };

    class GameState{

    };

    class GameManager{
        sf::RenderWindow* window;

        YAML::Node config;
        ctn::Board board;
        ctn::Graphics graphics;

        public:
        GameManager();
        GameManager(YAML::Node config_, sf::RenderWindow* window_);

        bool tick();

        void draw();
    };
}


#endif