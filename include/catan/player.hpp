#ifndef __CTN_PLAYER_HPP_
#define __CTN_PLAYER_HPP_

#include <map>
#include <string>
#include <optional>
#include "catan/constants.hpp"
#include "catan/board.hpp"
#include "wastils/input.hpp"

using str = std::string;

namespace ctn{
    struct PlayerInput{
        int object_type;
        int id;

        public:
        PlayerInput():
            object_type{-1},
            id{-1}
        {}

        void initialize(){object_type = -1; id = -1; };
    };


    class Player{
        std::map<str, int> owned_resources;
        std::map<str, int> owned_cards;
        int VP;
        str color;

        public:
        Player():
            owned_resources{},
            owned_cards{},
            VP{0},
            color{ctn::NONE}
        {} 

        Player(const str& color):
            owned_resources{},
            owned_cards{},
            VP{0},
            color{color}
        {}

        const str& get_color() const {return color; }

        void get_input(PlayerInput& ip, const Board& board);
    };
}

#endif