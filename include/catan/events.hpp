#ifndef __CATAN_EVENTS_HPP
#define __CATAN_EVEMTS_HPP

#include "catan/progress.hpp"
#include "wastils/input.hpp"
#include "catan/board.hpp"

namespace ctn{
    class EventManager{
        was::MouseManager* mouse;
        ctn::ProgressManager* progress;
        ctn::Board* board;

        public:
        EventManager() = default;
        EventManager(
            was::MouseManager* mouse_,
            ctn::ProgressManager* progress_,
            ctn::Board* board_):

            mouse(mouse_),
            progress(progress_),
            board(board_)
        {}

        void update();
        void build_validate();
    };
}

#endif