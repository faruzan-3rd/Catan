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
        bool
            setup_phase = true;

        int 
            finished_players_num = 0,
            last_placed_settlement = -1,
            tmp_current_player = -1;
        bool 
            second_tour = false,
            can_place_settlement = true,
            can_place_path = false,
            can_select_tile = false;
        int
            dice1_result = -1,
            dice2_result = -1;

        int 
            current_player = 0,
            selected_id = -1;

        bool resource_select = false;
        int resource_select_num = 0;
        std::map<str, int> selected_resources;

        SelectionType selection_type = SelectionType::None;

        GameState(){
            initialize_resource_selection();
        }

        void initialize_resource_selection(){
            for(str mat : ctn::materials){
                selected_resources[mat] = 0;
            }
        }

        int get_selected_resource(const str& name) const{
            if(selected_resources.count(name)){
                return selected_resources.at(name);
            }

            return 0;
        }

        int selected_total_resources() const{
            int out = 0;
            for(const str& mat : ctn::materials){
                out += get_selected_resource(mat);
            }
            return out;
        }
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

        int get_resource_quantity(const str& name) const{
            if(resources.count(name)){
                return resources.at(name);
            }
            
            return 0;
        }

        int get_total_resources() const{
            int out = 0;
            for(str mat : ctn::materials){
                out += get_resource_quantity(mat);
            }

            return out;
        }
    };

    struct PlayerEventCalls{
        std::function<bool(ctn::SelectionType, int, const str&)> select;
        std::function<bool()> confirm; 
        std::function<void()> roll_dices;
        std::function<void()> confirm_dices;

        PlayerEventCalls() = default;

        PlayerEventCalls(
            std::function<bool(ctn::SelectionType, int, const str&)> select_,
            std::function<bool()> confirm_,
            std::function<void()> roll_dices_,
            std::function<void()> confirm_dices_):
            select(select_),
            confirm(confirm_),
            roll_dices(roll_dices_),
            confirm_dices(confirm_dices_)
        {}
    };
}

#endif