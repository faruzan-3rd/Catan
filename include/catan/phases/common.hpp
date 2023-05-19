#ifndef __CATAN_PHASES_COMMON_HPP
#define __CATAN_PHASES_COMMON_HPP

#include "catan/progress.hpp"


// Collection of commonly used functions
namespace ctn::phases::common{
    void set_player_turn_txt(was::UIScheme* ui, int player_num);
    void set_order_txt(was::UIScheme* ui, const str& message);
    bool is_valid_settlement(bool setup);
    bool is_valid_path();
    bool is_valid_tile();
    void enable_ui(const str& name);
    void disable_ui(const str& name);
    void modify_board_control(int mask);
    void build_path();
    void build_settlement();
}


#endif