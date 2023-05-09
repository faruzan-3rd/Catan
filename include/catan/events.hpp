#ifndef __CATAN_EVENTS_HPP
#define __CATAN_EVEMTS_HPP

#include "catan/progress.hpp"
#include "wastils/input.hpp"
#include "catan/board.hpp"
#include "catan/graphics.hpp"
#include "catan/constants.hpp"

namespace ctn{
    class EventManager{
        was::MouseManager* mouse;
        ctn::ProgressManager* progress;
        ctn::Board* board;
        ctn::Graphics* graphics;

        public:
        EventManager() = default;
        EventManager(
            was::MouseManager* mouse_,
            ctn::ProgressManager* progress_,
            ctn::Board* board_,
            ctn::Graphics* graphics_):

            mouse(mouse_),
            progress(progress_),
            board(board_),
            graphics(graphics_)
        {}

        void update();
        bool select(SelectionType type, int id, const str& color);
        bool build_validate();
    };
}

#endif