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
        std::string tile_type;
        sf::RenderWindow* window;
        vec2f pos;
        int token;

        public:
        BoardTile():
            sprite{sf::Sprite()},
            tile_type{ctn::NONE},
            window{nullptr},
            pos{vec2f(0, 0)},
            token{-1}
        {}
        BoardTile(
                sf::RenderWindow* window_, 
                const sf::Sprite& sprite_, 
                const std::string& tile_type_, 
                const vec2f& pos_):
            sprite{sprite_},
            tile_type{tile_type_},
            window{window_},
            pos{pos_}
        {
            sprite.setPosition(pos);
        }

        void draw(){window->draw(sprite); }
        vec2f getPosition() const {return sprite.getPosition(); }
        int get_token() const {return token; }
        void set_token(const int& token_) {token = token_; }
        std::string get_tile_type() const {return tile_type; }

    };


    class TileRenderer{
        YAML::Node general_config, tiles_config, env_config, board_config;
        sf::RenderWindow* window_ptr;
        
        sf::Texture texture, env_texture;
        std::map<std::string, sf::Sprite> tiles;
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
        TileRenderer(){}
        ~TileRenderer() {if(tile_token_txt != nullptr) delete tile_token_txt; }
        
        void load_assets(const YAML::Node& config_, sf::RenderWindow* window_ptr_);

        void generate_tiles();

        std::vector<ctn::BoardTile> get_tiles() const;

        void draw();
    };
}


#endif