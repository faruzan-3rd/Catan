#include "catan/assets.hpp"


sf::Sprite ctn::load_sprite(const YAML::Node& config, sf::Texture& texture, float scale, int alpha){
    int width = config[2].as<int>();
    int height = config[3].as<int>();
    sf::Sprite sprite(texture, sf::IntRect(config[0].as<int>(), config[1].as<int>(), width, height));
    sprite.setScale(sf::Vector2f(scale, scale));
    sprite.setColor(sf::Color(255, 255, 255, alpha));

    return sprite;
}

std::map<std::string, sf::Sprite> ctn::load_sprites(const YAML::Node& config, sf::Texture& texture, const std::vector<std::string> load_list, float scale, int alpha, int offset_x, int offset_y, std::string prefix){
    std::map<std::string, sf::Sprite> ret;

    auto sprites = config["Sprites"];

    for(auto block_type : load_list){
        auto pos = sprites[block_type];
        int width = pos[2].as<int>();
        int height = pos[3].as<int>();
        sf::Sprite sprite(texture, sf::IntRect(pos[0].as<int>() + offset_x, pos[1].as<int>() + offset_y, width, height));
        sprite.setScale(sf::Vector2f(scale, scale));
        sprite.setColor(sf::Color(255, 255, 255, alpha));
        ret[prefix + block_type] = sprite;
    }

    return ret;
}

std::vector<sf::Sprite> ctn::load_from_list(const YAML::Node& config, sf::Texture& texture, float scale, int alpha){
    std::vector<std::vector<int>> sprites_rect;

    std::vector<sf::Sprite> sprites(sprites_rect.size());

    for(const YAML::Node elm : config){
        const std::vector<int>& rect = elm.as<std::vector<int>>();
        if(rect.size() != 4){
            std::cout << "Got an incomplete list of rects. Returning an incomplete sprite vector." << std::endl;
            return sprites;
        }

        sf::Sprite sprite(texture, sf::IntRect(rect[0], rect[1], rect[2], rect[3]));
        sprite.setScale(sf::Vector2f(scale, scale));
        sprite.setColor(sf::Color(255, 255, 255, alpha));
        sprites.push_back(sprite);
    }

    return sprites;
}
