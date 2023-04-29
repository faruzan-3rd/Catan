#ifndef __CTN__GMANAGER_HPP
#define __CTN__GMANAGER_HPP

#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"
#include "catan/board.hpp"
#include "catan/player.hpp"
#include "catan/progress.hpp"
#include "catan/events.hpp"
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
        was::UIScheme ui;
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

        void update(const was::MouseManager& mouse);

        void set_function(const str& ui_name, std::function<void()> func){
            ui.get_ptr_by_name(ui_name)->set_function(func);
        }
    };

    class Audio{

    };

    class GameState{

    };

    class GameManager{
        sf::RenderWindow* window;

        YAML::Node config;
        was::MouseManager mouse;
        ctn::Board board;
        ctn::Graphics graphics;
        ctn::ProgressManager progressmng;
        ctn::EventManager eventmanager;

        public:
        GameManager():
            window{nullptr},
            config{},
            mouse{},
            board{},
            graphics{},
            progressmng{},
            eventmanager{}
        {}
        GameManager(YAML::Node config_, sf::RenderWindow* window_);

        bool tick();

        void draw();

        private:
        void attribute_functions_to_ui();
    };
}


#endif