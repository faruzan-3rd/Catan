#ifndef __ASSETS_HPP_
#define __ASSETS_HPP_

#include <map>
#include <vector>
#include <iostream>
#include "catan/constants.hpp"
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"



namespace ctn{
    
    std::map<ctn::Tile, sf::Sprite> load_sprites(const YAML::Node& config, sf::Texture& texture, float scale=1.0, int alpha=255);

    std::vector<sf::Sprite> load_from_list(const YAML::Node& config, sf::Texture& texture, float scale=1.0, int alpha=255);
    
}


#endif