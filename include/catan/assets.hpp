#ifndef __ASSETS_HPP_
#define __ASSETS_HPP_

#include <map>
#include <vector>
#include <iostream>
#include "wastils/config.hpp"
#include "catan/constants.hpp"
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"



namespace ctn{
    sf::Sprite load_sprite(const YAML::Node& config, sf::Texture& texture, float scale=1.0, int alpha=255);

    std::map<std::string, sf::Sprite> load_sprites(const YAML::Node& config, sf::Texture& texture, const std::vector<std::string> load_list, float scale=1.0, int alpha=255, int offset_x=0, int offset_y=0, std::string prefix="");

    std::vector<sf::Sprite> load_from_list(const YAML::Node& config, sf::Texture& texture, float scale=1.0, int alpha=255);
    
}


#endif
