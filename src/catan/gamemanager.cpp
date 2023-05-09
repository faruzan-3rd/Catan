#include "catan/gamemanager.hpp"


ctn::GameManager::GameManager(YAML::Node config_, sf::RenderWindow* window_): GameManager()
{
    config = config_;
    window = window_;

    board.generate_board(config);
    board.generate_tiles(config);
    board.attribute_resources();
    board.generate_graph();
    board.generate_harbors();

    graphics.load_sprites(config, window);

    mouse = was::MouseManager(window);


    progressmng.graphics = &graphics;
    progressmng.board = &board;
    progressmng.playersmng = &playersmng;
    progressmng.ui = graphics.get_ui_ptr();
    progressmng.build_game_logic();
    eventmanager = EventManager(&mouse, &progressmng, &board, &graphics);
    PlayerEventCalls calls(
        std::bind(&EventManager::select, &eventmanager, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        std::bind(&EventManager::build_validate, &eventmanager),
        std::bind(&ProgressManager::execute_transition, &progressmng, "roll_dices")
    );
    playersmng.add_player(PlayerType::HUMAN, RED, calls);
    playersmng.add_player(PlayerType::HUMAN, BLUE, calls);
    playersmng.add_player(PlayerType::HUMAN, YELLOW, calls);

    attribute_functions_to_ui();

    progressmng.set_state("start");
    progressmng.execute_transition("start");
}

void ctn::GameManager::attribute_functions_to_ui(){
    graphics.set_function("validate_build", std::bind([this](){this->playersmng.UI_button_input(UIButton::Build, this->progressmng.gamestate); }));
    graphics.set_function("roll_dice", [this](){this->playersmng.UI_button_input(UIButton::DiceRoll, this->progressmng.gamestate); });
}

bool ctn::GameManager::tick(){
    /*
    1. INPUT CHECK
    */
    mouse.tick();
    sf::Event sf_event;
    while (window->pollEvent(sf_event))
    {
        if(sf_event.type == sf::Event::Closed){
            window->setActive(false);
            window->close();
            std::cout << "Quit" << std::endl;
            return false;
        }

        mouse.reg_event(sf_event);
    }

    /*
    2. UI/AI INPUT
    */
    graphics.update(mouse);
    playersmng.update(mouse, board, progressmng.gamestate);
    
    draw();

    return 0;
}

void ctn::GameManager::draw(){
    graphics.draw(&board, playersmng.get_current_player_info(progressmng.gamestate.current_player));
}
