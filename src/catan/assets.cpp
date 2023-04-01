#include "catan/assets.hpp"


std::map<ctn::Tile, sf::Sprite> ctn::load_sprites(const YAML::Node& config, sf::Texture& texture){
    std::map<ctn::Tile, sf::Sprite> ret;

    int scale = config["scale"].as<int>();
    int alpha = config["alpha"].as<int>();

    auto sprites = config["Sprites"];

    for(auto block_type : ctn::All_types){
        auto pos = sprites[ctn::enum2str.at(block_type)];
        int width = pos[2].as<int>();
        int height = pos[3].as<int>();
        sf::Sprite sprite(texture, sf::IntRect(pos[0].as<int>(), pos[1].as<int>(), width, height));
        sprite.setScale(sf::Vector2f(scale, scale));
        sprite.setColor(sf::Color(255, 255, 255, alpha));
        ret[block_type] = sprite;
    }

    return ret;
}