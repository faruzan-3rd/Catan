#include "catan/gamemanager.hpp"

std::vector<ctn::Player> players;

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

    players.push_back(Player(ctn::RED));
    players.push_back(Player(ctn::BLUE));
    players.push_back(Player(ctn::GREEN));

    mouse = was::MouseManager(window);

    progressmng.graphics = &graphics;
    progressmng.board = &board;
    progressmng.ui = graphics.get_ui_ptr();
    progressmng.build_game_logic();
    eventmanager = EventManager(&mouse, &progressmng, &board, &graphics);

    attribute_functions_to_ui();

    progressmng.set_state("start");
    progressmng.execute_transition("start");
}

void ctn::GameManager::attribute_functions_to_ui(){
    graphics.set_function("validate_build", std::bind(&EventManager::build_validate, &eventmanager));
    graphics.set_function("roll_dice", std::bind(&ProgressManager::execute_transition, &progressmng, "roll_dices"));
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
    eventmanager.update();
    
    draw();

    return 0;
}

void ctn::GameManager::draw(){
    graphics.draw(&board);
}
