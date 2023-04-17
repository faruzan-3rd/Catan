#ifndef __CTN_PLAYER_HPP_
#define __CTN_PLAYER_HPP_

#include <map>
#include <string>
#include "catan/constants.hpp"

typedef std::string str;

namespace ctn{
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

        str get_color() const {return color; }
    };
}

#endif