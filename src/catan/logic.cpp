#include "catan/progress.hpp"


bool is_valid_settlement(ctn::ProgressManager* progress, bool setup){
    if(progress->selection_type != ctn::PLACE) return false;

    return progress->board->can_build_settlement_here(players[progress->current_player].get_color(), progress->selected_id, setup);
}

bool is_valid_path(ctn::ProgressManager* progress){
    if(progress->selection_type != ctn::PATH) return false;

    auto selected_path = progress->selected_id;
    auto current_color = players[progress->current_player].get_color();
    return progress->board->can_build_path_here(current_color, selected_path);
}

void build_settlement(ctn::ProgressManager* progress){
    progress->board->build_settlement(progress->selected_id, ctn::HOUSE, players[progress->current_player].get_color());

    if(progress->gamestate.setup_phase){
        if(progress->gamestate.second_tour){
            std::cout << "Generate resource!" << std::endl;
        }

        progress->gamestate.can_place_path = true;
        progress->gamestate.can_place_settlement = false;
    }
}

void build_path(ctn::ProgressManager* progress){
    progress->board->build_path(progress->selected_id, players[progress->current_player].get_color());

    if(progress->gamestate.setup_phase){
        progress->gamestate.can_place_path = false;
        progress->gamestate.finished_players_num++;
        if(progress->gamestate.finished_players_num == (int)players.size()){
            if(progress->gamestate.second_tour){
                progress->gamestate.setup_phase = false;
                return;
            }
            else{
                progress->gamestate.second_tour = true;
                progress->gamestate.finished_players_num = 0;
            }
        }
        else{
            progress->current_player = (progress->current_player + (progress->gamestate.second_tour ? -1 : 1) + players.size()) % players.size();
        }

        progress->gamestate.can_place_settlement = true;
    }
    else{
        // progress->current_player++;
        // progress->current_player %= players.size();
    }
}

void ctn::ProgressManager::build_game_logic(){
    auto print = [](const std::string& msg){std::cout << msg << std::endl; };
    add_state(State("start", std::bind(print, "start")));
    add_state(State("setup_settlement", std::bind(print, "setup settlement")));
    add_state(State("setup_path", std::bind(print, "setup path")));

    add_transition(Transition(
        "start",
        "start",
        "setup_settlement",
        [](){return true;},
        std::bind(print, "Start")
    ));

    add_transition(Transition(
        "setup_validate_settlement", 
        "setup_settlement", 
        "setup_path", 
        std::bind(is_valid_settlement, this, true),
        std::bind(build_settlement, this)));

    add_transition(Transition(
        "setup_validate_path", 
        "setup_path", 
        "setup_settlement", 
        std::bind(is_valid_path, this),
        std::bind(build_path, this)));
}