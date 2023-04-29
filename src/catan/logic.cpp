#include "catan/progress.hpp"


bool is_valid_settlement(ctn::ProgressManager* progress, bool setup){
    if(progress->selection_type != ctn::PLACE) return false;

    auto graph = progress->board->get_graph();
    auto settlements = progress->board->get_places();
    auto paths = progress->board->get_paths();
    bool ok = false;
    // Condition 1: Needs to be empty
    if(settlements[progress->selected_id].get_type() != ctn::NONE){
        return false;
    }

    for(auto path : graph[progress->selected_id]){
        // Condition 2: No other settlement around
        if(settlements[path.to].get_type() != ctn::NONE){
            return false;
        }

        // Condition 3: Connected to a path(except for setup phase)
        if(setup) ok = true;
        else{
            if(paths[path.path_id].get_color() == players[progress->current_player].get_color()){
                ok = true;
            }
        }
    }

    return ok;
}

bool is_valid_path(ctn::ProgressManager* progress){
    if(progress->selection_type != ctn::PATH) return false;

    auto graph = progress->board->get_graph();
    auto settlements = progress->board->get_places();
    auto paths = progress->board->get_paths();
    auto selected_path = paths[progress->selected_id];
    auto current_color = players[progress->current_player].get_color();

    // Condition 1: empty
    if(selected_path.get_color() != ctn::INVISIBLE){
        return false;
    }

    // Condition 2: Adjacent settlement
    if(settlements[selected_path.get_place1()].get_color() == current_color || settlements[selected_path.get_place2()].get_color() == current_color){
        return true;
    }

    // Condition 3: Adjacent path
    for(auto path_data : graph[selected_path.get_place1()]){
        if(paths[path_data.path_id].get_color() == current_color){
            return true;
        }
    }
    for(auto path_data : graph[selected_path.get_place2()]){
        if(paths[path_data.path_id].get_color() == current_color){
            return true;
        }
    }

    return false;
}

void build_settlement(ctn::ProgressManager* progress){
    progress->board->build_settlement(progress->selected_id, ctn::HOUSE, players[progress->current_player].get_color());

    if(progress->setupdata.second_tour && progress->setupdata.setup_phase){
        std::cout << "Generate resource!" << std::endl;
    }
}

void build_path(ctn::ProgressManager* progress){
    progress->board->build_path(progress->selected_id, players[progress->current_player].get_color());

    if(progress->setupdata.setup_phase){
        if(progress->setupdata.finished_players_num == (int)players.size()){
            if(progress->setupdata.second_tour){
                progress->setupdata.setup_phase = false;
            }
            else{
                progress->setupdata.second_tour = true;
                progress->setupdata.finished_players_num = 0;
            }
        }
        else{
            progress->current_player = (progress->current_player + (progress->setupdata.second_tour ? -1 : 1) + players.size()) % players.size();
            progress->setupdata.finished_players_num++;
        }
    }
    else{
        progress->current_player++;
        progress->current_player %= players.size();
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