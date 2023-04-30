#ifndef __CATAN_PROGRESS_HPP
#define __CATAN_PROGRESS_HPP

#include <functional>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "catan/board.hpp"
#include "catan/player.hpp"
#include "wastils/UI.hpp"

extern std::vector<ctn::Player> players;


namespace ctn{
    struct GameState{
        bool setup_phase = true;
        int finished_players_num = 0;
        bool 
            second_tour = false,
            can_place_settlement = true,
            can_place_path = false;

        GameState() = default;
    };

    class State{
        std::string name;
        std::function<void()> onEnter;

        public:
        State() = default;

        State(const std::string& name_, const std::function<void()>& onEnter_):
            name{name_},
            onEnter{onEnter_}
        {}

        const std::string& get_name() const {return name; }
        void on_enter(){onEnter(); }
    };

    class Transition{
        std::string name;
        std::string fromState, toState;
        std::function<bool()> condition;
        std::function<void()> callback;

        public:
        Transition() = default;

        Transition(
            const std::string& name_,
            const std::string& fromStateName,
            const std::string& toStateName,
            const std::function<bool()>& condition_,
            const std::function<void()>& callback_):
            
            name{name_},
            fromState{fromStateName},
            toState{toStateName},
            condition{condition_},
            callback{callback_}
        {}

        const std::string& get_name() const{return name; }
        const std::string& get_from_state() const{return fromState; }
        const std::string& get_to_state() const {return toState; }
        bool can_execute(){return condition(); }
        void execute_callback(){callback(); }
    };

    class ProgressManager{  // FSM
        std::map<std::string, State> states;
        std::string current_state;
        std::map<std::string, Transition> transitions;

        public:
        GameState gamestate;
        Board* board;
        was::UIScheme* ui;

        int current_player = 0;
        int selected_id = -1;
        int selection_type = -1;

        public:
        ProgressManager() = default;
        
        void add_state(const State& state){states[state.get_name()] = state; }
        void add_transition(const Transition& transition){transitions[transition.get_name()] = transition; };
        bool execute_transition(const std::string& transition_name){
            if(transitions[transition_name].get_from_state() != current_state || !transitions[transition_name].can_execute()) return false;

            transitions[transition_name].execute_callback();
            current_state = transitions[transition_name].get_to_state();
            states[current_state].on_enter();

            return true;
        }
        void set_state(const std::string& name){current_state = name; }
        void build_game_logic();
    };
}



#endif