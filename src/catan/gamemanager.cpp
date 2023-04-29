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

    progressmng.board = &board;
    progressmng.build_game_logic();
    eventmanager = EventManager(&mouse, &progressmng, &board);

    attribute_functions_to_ui();

    progressmng.set_state("start");
    progressmng.execute_transition("start");
}

void ctn::GameManager::attribute_functions_to_ui(){
    graphics.set_function("validate", std::bind(&EventManager::build_validate, &eventmanager));
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
    graphics.draw(
        board.get_tiles(),
        board.get_places(),
        board.get_harbors(),
        board.get_paths());
}
