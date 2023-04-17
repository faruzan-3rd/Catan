#ifndef __CTN_TURN_HPP_
#define __CTN_TURN_HPP_

#include "catan/constants.hpp"
#include "catan/message.hpp"
#include "catan/player.hpp"
#include <map>
#include <vector>
#include <random>
#include <iostream>

typedef std::string str;
extern ctn::Message message;
extern std::vector<ctn::Player> players;

namespace ctn {
    // Responsible for setup and turn management
    class TurnManager{
        int current_player;
        int setup_done_player_num;
        int start_with;
        bool setup_phase;
        bool setup_2nd_turn;

        public:
        TurnManager():
            current_player{0},
            setup_done_player_num{0},
            start_with{0},
            setup_phase{true},
            setup_2nd_turn{false}
        {}

        private:
        void setup();
        
        public:
        void tick();

    };
}

#endif