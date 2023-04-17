#ifndef __CTN_PROGRESS_HPP_
#define __CTN_PROGRESS_HPP_


#include "catan/message.hpp"
#include "catan/player.hpp"
#include "catan/board.hpp"
#include <vector>
#include <iostream>
#include <optional>
#include "SFML/Graphics.hpp"
#include "wastils/events.hpp"
extern ctn::Message message;
extern std::vector<ctn::Player> players;
extern std::optional<was::EventManager> eventmgr;


namespace ctn{
    class ProgressManager{

        public:
        void tick(Board* board);
    };
}



#endif