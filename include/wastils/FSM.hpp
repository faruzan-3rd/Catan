#ifndef __WASTILS_FSM_HPP
#define __WASTILS_FSM_HPP

#include <functional>
#include <string>
#include <vector>
#include <map>

namespace was{
    template <typename T>
    class State{
        std::string name;
        std::function<void(T&)> onEnter;

        public:
        State() = default;

        State(const std::string& name_, const std::function<void(T)>& onEnter_):
            name{name_},
            onEnter{onEnter_}
        {}

        const std::string& get_name() const {return name; }
        void on_enter(T& param){onEnter(param); }
    };

    template <typename T>
    class Transition{
        std::string name;
        std::string fromState, toState;
        std::function<bool(T)> condition;
        std::function<void()> callback;

        public:
        Transition() = default;

        Transition(
            const std::string& name_,
            const std::string& fromStateName,
            const std::string& toStateName,
            const std::function<bool(T)>& condition_,
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
        bool can_execute(T& param){return condition(param); }
        void execute_callback(){callback(); }
    };

    template<typename StateParamType, typename TransitionParamType>
    class FSM{
        std::map<std::string, State<StateParamType>> states;
        std::string current_state;
        std::map<std::string, Transition<TransitionParamType>> transitions;

        public:
        FSM() = default;

        void add_state(const State<StateParamType>& state){states.insert(states[state.get_name()] = state); }
        void add_transition(const Transition<TransitionParamType>& transition){transitions[transition.get_name()] = transition; };
        bool execute_transition(const std::string& transition_name, StateParamType& state_param, TransitionParamType& transition_param){
            if(transitions[transition_name].get_from_state() != current_state || !transitions[transition_name].can_execute(transition_param)) return false;

            transitions[transition_name].execute_callback();
            current_state = transitions[transition_name].get_to_state();
            states[current_state].on_enter(state_param);

            return true;
        }
        void set_state(const std::string& name){current_state = name; }
    };
}


#endif