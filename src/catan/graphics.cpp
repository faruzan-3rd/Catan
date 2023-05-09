#include "catan/graphics.hpp"


void ctn::Graphics::load_sprites(const YAML::Node& config_, sf::RenderWindow* window_){
    window = window_;
    YAML::Node assets_cfg, board_cfg;
    if(was::load_config(assets_cfg, config_["Game"]["assets"].as<str>())){
        std::cout << "Error while loading assets config. (graphics)" << std::endl;
        return;
    }
    if(was::load_config(board_cfg, config_["Game"]["board"].as<str>())){
        std::cout << "Error while loading board config. (graphics)" << std::endl;
        return;
    }

    std::cout << "Loading textures..." << std::endl;
    if(!tx_board_objects.loadFromFile(assets_cfg["Board"]["texture"].as<str>())){
        std::cout << "Board texture load failed" << std::endl;
    }
    if(!tx_environment.loadFromFile(assets_cfg["Environment"]["texture"].as<str>())){
        std::cout << "Environment texture load failed" << std::endl;
    }
    if(!tx_tilemap.loadFromFile(assets_cfg["Tiles"]["texture"].as<str>())){
        std::cout << "Tilemap texture load failed" << std::endl;
    }
    if(!tx_dice.loadFromFile(assets_cfg["Dice"]["texture"].as<str>())){
        std::cout << "Dice texture load failed" << std::endl;
    }
    if(!tx_ui.loadFromFile(assets_cfg["UI"]["texture"].as<str>())){
        std::cout << "UI texture load failed" << std::endl;
    }
    ft_mario.loadFromFile(assets_cfg["Test"]["font"].as<str>());
    std::cout << "Textures loaded!" << std::endl;


    std::cout << "Loading board objects..." << std::endl;
    std::vector<str> building_types = ctn::house_types + ctn::path_types;
    std::map<str, sprt> tmp, tmp_transparent;
    float alpha_preview = assets_cfg["Board"]["Buildings"]["preview_alpha"].as<float>();
    for(str color : ctn::colors){
        int
            offset_x = assets_cfg["Board"]["Buildings"]["Color-offsets"][color][0].as<int>(),
            offset_y = assets_cfg["Board"]["Buildings"]["Color-offsets"][color][1].as<int>();

        tmp = ctn::load_sprites(
                assets_cfg["Board"]["Buildings"],
                tx_board_objects, 
                building_types, 
                1.0f, 
                255, 
                offset_x, 
                offset_y, 
                color);
        tmp_transparent = ctn::load_sprites(
            assets_cfg["Board"]["Buildings"],
                tx_board_objects, 
                building_types, 
                1.0f, 
                255 * alpha_preview, 
                offset_x, 
                offset_y, 
                color
        );
        for(auto p : tmp){
            sp_board_obj[p.first] = p.second;
        }
        for(auto p : tmp_transparent){
            sp_board_obj_preview[p.first] = p.second;
        }
    }
    sp_board_obj[ctn::NONE] = ctn::load_sprite(assets_cfg["Board"]["Buildings"]["Sprites"]["none"], tx_board_objects);
    sp_board_obj[ctn::TOKEN] = ctn::load_sprite(assets_cfg["Board"]["Other"]["token"], tx_board_objects);
    sp_board_obj[ctn::ROBBER] = ctn::load_sprite(assets_cfg["Board"]["Other"]["robber"], tx_board_objects);
    gracfg.token_offset = vec2f(
        board_cfg["token_offset"][0].as<int>(),
        board_cfg["token_offset"][1].as<int>()
    );
    gracfg.token_txt_offset = vec2f(
        board_cfg["token_txt_offset"][0].as<int>(),
        board_cfg["token_txt_offset"][1].as<int>()
    );
    gracfg.robber_offset = vec2f(
        board_cfg["robber-offset"][0].as<int>(),
        board_cfg["robber-offset"][1].as<int>()
    );
    txt_token = std::unique_ptr<was::Text>(
        new was::Text(
            "",
            ft_mario,
            window,
            board_cfg["token_txt_size"].as<int>(),
            sf::Color::Black,
            vec2f(0, 0))
    );
    sp_dices = ctn::load_sprites(assets_cfg["Dice"], tx_dice, ctn::dice_faces, 2.0f);
    gracfg.dice1_pos = vec2f(
        board_cfg["dice1-pos"][0].as<int>(),
        board_cfg["dice1-pos"][1].as<int>()
    );
    gracfg.dice2_pos = vec2f(
        board_cfg["dice2-pos"][0].as<int>(),
        board_cfg["dice2-pos"][1].as<int>()
    );
    std::cout << "Board objects loaded!" << std::endl;


    std::cout << "Loading harbor stuff..." << std::endl;
    std::vector<str> mats = ctn::materials;
    mats.push_back(ctn::ANY);
    sp_harbor = ctn::load_sprites(assets_cfg["Board"]["Harbor"], tx_board_objects, mats);
    sp_harbor[ctn::HARBOR] = ctn::load_sprite(
                                assets_cfg["Board"]["Harbor"]["sprite"], 
                                tx_environment,
                                1.5f);
    std::cout << "Harbor stuff loaded!" << std::endl;

    std::cout << "Loading tilemap..." << std::endl;
    sp_tiles = ctn::load_sprites(
                    assets_cfg["Tiles"], 
                    tx_tilemap, ctn::tile_types, 
                    assets_cfg["Tiles"]["scale"].as<float>());
    gracfg.sea_dimension = vec2i(
        assets_cfg["Environment"]["Sea"]["size"][0].as<int>(),
        assets_cfg["Environment"]["Sea"]["size"][1].as<int>()
    );
    sp_tiles[ctn::SEA] = ctn::load_sprite(
                            assets_cfg["Environment"]["Sea"]["sprite"],
                            tx_environment,
                            assets_cfg["Environment"]["Sea"]["scale"].as<float>()
                            );

    std::cout << "Tilemap loaded!" << std::endl;

    YAML::Node ui_scheme;
    was::load_config(ui_scheme, config_["Game"]["ui"].as<str>());
    ui = was::UIScheme(window);
    ui.load(ui_scheme["game"]);

    YAML::Node player_info_config;
    was::load_config(player_info_config, config_["Game"]["player_info"].as<str>());
    player_info = was::UIScheme(window);
    player_info.load(player_info_config["player0"]);

    sp_players = ctn::load_sprites(assets_cfg["UI"]["player_icns"], tx_ui,
                                    {ctn::YELLOW, ctn::RED, ctn::GREEN, ctn::BLUE});
}


void ctn::Graphics::draw(const Board* board, PlayerInfo player)
    {
    const std::vector<ctn::BoardTile>& tiles = board->get_tiles();
    const std::vector<ctn::Place>& places = board->get_places();
    const std::vector<ctn::Harbor>& harbors = board->get_harbors();
    const std::vector<ctn::Path>& paths = board->get_paths();
    int robber_tile = board->get_robber_tile();

    // 1. Sea (background)
    for(int x = 0; x < gracfg.sea_dimension.x; x++){
        for(int y = 0; y < gracfg.sea_dimension.y; y++){
            vec2f position(
                x * sp_tiles[ctn::SEA].getTextureRect().width,
                y * sp_tiles[ctn::SEA].getTextureRect().height
            );

            sp_tiles[ctn::SEA].setPosition(position);
            window->draw(sp_tiles[ctn::SEA]);
        }
    }

    // 2. Material tiles and tokens
    for(const ctn::BoardTile& tile : tiles){
        str tile_type = tile.get_tile_type();
        sp_tiles[tile_type].setPosition(tile.get_position());
        window->draw(sp_tiles[tile_type]);

        if(tile.get_token() == 0) continue;

        sp_board_obj[ctn::TOKEN].setPosition(tile.get_position() + gracfg.token_offset);
        window->draw(sp_board_obj[ctn::TOKEN]);

        txt_token->set_text(std::to_string(tile.get_token()));
        txt_token->set_position(tile.get_position() + gracfg.token_txt_offset);
        txt_token->draw();
    }
    sp_board_obj[ctn::ROBBER].setPosition(tiles[robber_tile].get_position() + gracfg.robber_offset);
    window->draw(sp_board_obj[ctn::ROBBER]);


    // 3. Harbor stuff
    for(const ctn::Harbor& harbor : harbors){
        sp_harbor[ctn::HARBOR].setPosition(harbor.get_position());
        sp_harbor[ctn::HARBOR].setRotation(harbor.get_rotation());
        str material = harbor.get_required_mat();
        sp_harbor[material].setPosition(harbor.get_mat_position());

        window->draw(sp_harbor[ctn::HARBOR]);
        window->draw(sp_harbor[material]);
    }

    // 4. Settlements and paths
    for(const ctn::Place& place : places){
        str house_name = place.get_color() + place.get_type();
        sp_board_obj[house_name].setPosition(place.get_position());
        window->draw(sp_board_obj[house_name]);
    }
    for(const ctn::Path& path : paths){
        str path_name = path.get_color() + path.get_path_type();
        sp_board_obj[path_name].setPosition(path.get_position());
        window->draw(sp_board_obj[path_name]);
    }
    if(preview_type != ctn::NONE){
        sp_board_obj_preview[preview_type].setPosition(preview_pos);
        window->draw(sp_board_obj_preview[preview_type]);
    }

    // 5. Other
    if(gracfg.dices_enabled){
        sp_dices[ctn::dice_faces[gracfg.dice_1 - 1]].setPosition(gracfg.dice1_pos);
        window->draw(sp_dices[ctn::dice_faces[gracfg.dice_1 - 1]]);
        sp_dices[ctn::dice_faces[gracfg.dice_2 - 1]].setPosition(gracfg.dice2_pos);
        window->draw(sp_dices[ctn::dice_faces[gracfg.dice_2 - 1]]);
    }

    // 6. UI
    ui.draw();

    str name = "Player " + str(1, '0' + player.id);
    player_info.get_ptr_by_name("cur_player_name")->set_text(name);
    sf::Sprite icon = sp_players[player.color];
    icon.setPosition(player_info.get_ptr_by_name("icon")->get_position());
    player_info.get_ptr_by_name("icon")->set_new_image(tx_ui, icon);
    for(const str& material : ctn::materials){
        player_info.get_ptr_by_name(material + "_num")->set_text(std::to_string(player.get_resource_quantity(material)));
    }
    player_info.draw();
}

void ctn::Graphics::update(const was::MouseManager& mouse){
    ui.update(mouse);
}

void ctn::Graphics::set_preview_build(const str& color, SelectionType type, int id, Board* board){
    if(type == SelectionType::None){
        preview_type = ctn::NONE;
        preview_pos = vec2f(0, 0);
    }
    else if(type == SelectionType::Settlement){
        preview_type = color + ctn::HOUSE;
        preview_pos = board->get_places()[id].get_position();
        
    }
    else if(type == SelectionType::Path){
        preview_type = color + board->get_paths()[id].get_path_type();
        preview_pos = board->get_paths()[id].get_position();
    }
    else{
        std::cout << "Preview failed" << std::endl;
    }
}