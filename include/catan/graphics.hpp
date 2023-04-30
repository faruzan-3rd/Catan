#ifndef __CATAN_GRAPHICS_HPP
#define __CATAN_GRAPHICS_HPP

#include "catan/board.hpp"

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
            sp_board_obj_preview,
            sp_harbor;
        
        // Font
        sf::Font ft_mario;

        // UI
        was::UIScheme ui;
        std::unique_ptr<was::Text>
            txt_token;

        // Some config
        ctn::graphicscfg gracfg;
        str preview_type = ctn::NONE;
        vec2f preview_pos;

        public:
        Graphics() = default;

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

        void set_preview_build(const str& color, int build_type, int id, Board* board, bool setup);

        was::UIScheme* get_ui_ptr() {return &ui; }
    };
}

#endif