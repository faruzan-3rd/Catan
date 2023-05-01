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
            token_txt_offset,
            robber_offset;

        vec2f 
            dice1_pos,
            dice2_pos;

        bool dices_enabled = false;
        int
            dice_1 = 1,
            dice_2 = 1;

        vec2i
            u,
            v,
            sea_dimension;

        graphicscfg() = default;
    };

    class Graphics{
        sf::RenderWindow* window;

        // Textures
        sf::Texture
            tx_board_objects,
            tx_environment,
            tx_tilemap,
            tx_dice;

        // In-game sprite map
        std::map<str, sprt>
            sp_tiles,
            sp_board_obj,
            sp_board_obj_preview,
            sp_harbor,
            sp_dices;
        
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

        void draw(const Board* board);

        void update(const was::MouseManager& mouse);

        void set_function(const str& ui_name, std::function<void()> func){
            ui.get_ptr_by_name(ui_name)->set_function(func);
        }

        void set_preview_build(const str& color, int build_type, int id, Board* board);

        void enable_dice(int dice_1, int dice_2){gracfg.dice_1 = dice_1; gracfg.dice_2 = dice_2; gracfg.dices_enabled = true; }

        void disable_dice(){gracfg.dices_enabled = false; };

        was::UIScheme* get_ui_ptr() {return &ui; }
    };
}

#endif