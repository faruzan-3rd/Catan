#ifndef __CATAN_SHARED_HPP
#define __CATAN_SHARED_HPP

#include <functional>
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "catan/constants.hpp"

using str = std::string;

namespace ctn{
    struct GameState{
        bool setup_phase = true;
        int 
            finished_players_num = 0,
            last_placed_settlement = -1;
        bool 
            second_tour = false,
            can_place_settlement = true,
            can_place_path = false;
        int
            dice1_result = -1,
            dice2_result = -1;

        int 
            current_player = 0,
            selected_id = -1;
        SelectionType selection_type = SelectionType::None;

        GameState() = default;
    };

    struct PlayerInfo{
        const int id;
        const str& color;
        const std::map<str, int>& resources;

        PlayerInfo() = default;

        PlayerInfo(int id_, const str& color_, const std::map<str, int>& resources_):
            id(id_),
            color(color_),
            resources(resources_)
        {}

        int get_resource_quantity(const str& name){
            if(resources.count(name)){
                return resources.at(name);
            }
            
            return 0;
        }
    };

    struct PlayerEventCalls{
        std::function<bool(ctn::SelectionType, int, const str&)> select;
        std::function<bool()> confirm; 
        std::function<void()> roll_dices;

        PlayerEventCalls() = default;

        PlayerEventCalls(
            std::function<bool(ctn::SelectionType, int, const str&)> select_,
            std::function<bool()> confirm_,
            std::function<void()> roll_dices_):
            select(select_),
            confirm(confirm_),
            roll_dices(roll_dices_)
        {}
    };
}

#endif