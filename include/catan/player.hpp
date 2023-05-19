#ifndef __CTN_PLAYER_HPP_
#define __CTN_PLAYER_HPP_

#include <map>
#include <string>
#include <optional>
#include <vector>
#include "wastils/input.hpp"
#include "catan/constants.hpp"
#include "catan/board.hpp"
#include "catan/shared.hpp"

using str = std::string;

namespace ctn{
    class Player{
        std::map<str, int> owned_resources;
        std::map<str, int> owned_cards;
        int VP;
        str color;
        int id;
        PlayerEventCalls calls;

        public:
        Player():
            owned_resources{},
            owned_cards{},
            VP{0},
            color{ctn::NONE},
            id{-1},
            calls{}
        {} 

        Player(const str& color, int id_, PlayerEventCalls calls_):
            owned_resources{},
            owned_cards{},
            VP{0},
            color{color},
            id{id_},
            calls{calls_}
        {
            // tmp
            owned_resources[ctn::WOOD] = 4;
            owned_resources[ctn::WHEAT] = 3;
            owned_resources[ctn::BRICK] = 2;
        }

        const str& get_color() const {return color; }

        int get_id() const {return id; }

        void gain_resource(const str& resource_type, int num=1){
            owned_resources[resource_type] += num;
        }

        virtual void update(const was::MouseManager& mouse, const Board& board, const GameState& state);

        void UI_button_input(UIButton button);

        PlayerInfo get_info() const;
    };


    class PlayersManager{
        std::vector<Player> players;

        public:
        PlayersManager() = default;
        
        void add_player(PlayerType type, const std::string& color, PlayerEventCalls calls){
            players.push_back(Player(color, players.size(), calls));
        }

        void update(const was::MouseManager& mouse, const Board& board, const GameState& state){
            players[state.current_player].update(mouse, board, state);
        }

        int get_players_num() const{return players.size(); }

        void UI_button_input(UIButton button, const GameState& state){
            players[state.current_player].UI_button_input(button);
        }

        const str& get_current_color(int current_player) const{
            return players[current_player].get_color();
        }

        void give_resource(int current_player, const str& resource_type, int num=1){
            players[current_player].gain_resource(resource_type, num);
        }

        const PlayerInfo get_player_info(int current_player) const{
            return players[current_player].get_info();
        }
    };
}

#endif