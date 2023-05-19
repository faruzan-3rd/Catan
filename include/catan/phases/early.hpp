#ifndef __CATAN_PHASES_EARLY_HPP
#define __CATAN_PHASES_EARLY_HPP

#include "catan/progress.hpp"

// Functions used in early phase, i.e. Dice phase, robber phase
namespace ctn::phases::early{
    int find_next_robbed_player();
    bool resource_go_next();
    void rob();
    void on_start();
    bool is_valid_resource_selection(bool exit_transition);
    void interpret_dices();
    void to_roll_dice();
    void to_select_tile();  
    void to_resource_select();
    void to_resource_next_player();
}


#endif