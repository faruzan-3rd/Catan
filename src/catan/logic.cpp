#include "catan/progress.hpp"
#include "catan/phases/common.hpp"
#include "catan/phases/early.hpp"
#include "catan/phases/trade.hpp"

using namespace ctn::phases;
ctn::ProgressManager* prog_ptr;

ctn::ProgressManager::ProgressManager(){
    prog_ptr = this;
}


void ctn::ProgressManager::build_game_logic(){
    auto print = [](const std::string& msg){std::cout << msg << std::endl; };
    auto empty = [](){};
    add_state(State("start", empty));
    add_state(State("setup_settlement", empty));
    add_state(State("setup_path", empty));
    add_state(State("dice_roll", early::interpret_dices));
    add_state(State("select_tile", std::bind(print, "+++++Select tile+++++")));
    add_state(State("select_trade_action", trade::on_enter_trade));
    add_state(State("robber_tile_select", early::to_select_tile));
    add_state(State("robber_resource_select", std::bind(print, "+++++Select resources+++")));

    add_transition(Transition(
        "start",
        "start",
        "setup_settlement",
        [](){return true;},
        early::on_start
    ));

    add_transition(Transition(
        "setup_validate_settlement", 
        "setup_settlement", 
        "setup_path", 
        std::bind(common::is_valid_settlement, true),
        common::build_settlement));

    add_transition(Transition(
        "setup_validate_path", 
        "setup_path", 
        "setup_settlement", 
        common::is_valid_path,
        common::build_path));

    add_transition(Transition(
        "roll_dices",
        "setup_settlement",
        "dice_roll",
        [this](){return !this->gamestate.setup_phase;},
        early::to_roll_dice
    ));

    add_transition(Transition(
        "enter_tile_select",
        "dice_roll",
        "robber_tile_select",
        [this](){return this->gamestate.dice1_result + this->gamestate.dice2_result == 7; },
        empty
    ));

    add_transition(Transition(
        "enter_resource_select",
        "robber_tile_select",
        "robber_resource_select",
        common::is_valid_tile,
        early::to_resource_select
    ));

    add_transition(Transition(
        "go_next_player_robber",
        "robber_resource_select",
        "robber_resource_select",
        std::bind(early::is_valid_resource_selection, false),
        early::to_resource_next_player
    ));

    add_transition(Transition(
        "enter_trade_action_select_from_robber",
        "robber_resource_select",
        "select_trade_action",
        std::bind(early::is_valid_resource_selection, true),
        [this](){this->gamestate.current_player = this->gamestate.tmp_current_player; }
    ));

    add_transition(Transition(
        "nobody_to_rob",
        "robber_resource_select",
        "select_trade_action",
        [](){return true; },
        empty
    ));

    add_transition(Transition(
        "enter_trade_action_select",
        "dice_roll",
        "select_trade_action",
        [this](){return this->gamestate.dice1_result + this->gamestate.dice2_result != 7; },
        empty
    ));
}