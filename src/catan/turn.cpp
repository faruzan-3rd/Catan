#include "catan/turn.hpp"


void ctn::TurnManager::setup(){
    if(message.verb != "placed") return;

    if(message.what == "path"){
        setup_done_player_num++;

        if(setup_done_player_num == players.size()){
            if(setup_2nd_turn){
                std::cout << "Finished setup" << std::endl;
                message = {"turnmng", -1, "finished", "", "setup", ""};
                return;
            }
            else{
                setup_2nd_turn = true;
                setup_done_player_num = 0;
                std::cout << "Starting 2nd lap" << std::endl;
                message = {"turnmng", current_player, "placing", ctn::HOUSE, "setup", "2"};
                return;
            }
        }
        else{ // If the current lap is not finished
            current_player = (current_player + (setup_2nd_turn ? -1 : 1) + players.size()) % players.size();
            message = {"turnmng", current_player, "placing", ctn::HOUSE, "setup", (setup_2nd_turn ? "2" : "1")};
        }
    }
    else{ // If a house is placed
        message.verb = "placing";
        message.what = "path";
    }
}


void ctn::TurnManager::tick(){
    // SETUP
    if(message.sender == "init"){
        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        std::uniform_int_distribution<int> rand(0, players.size() - 1);

        start_with = rand(engine);
        current_player = start_with;

        message = {"turnmng", start_with, "placing", ctn::HOUSE, "setup", "1"};

        std::cout << "Starting setup with player " << start_with << std::endl;
        return;
    }
    setup();


}