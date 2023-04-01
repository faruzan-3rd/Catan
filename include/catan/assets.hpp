#ifndef __ASSETS_HPP_
#define __ASSETS_HPP_

#include <map>
#include <iostream>
#include "catan/constants.hpp"
#include "yaml-cpp/yaml.h"
#include "SFML/Graphics.hpp"



namespace ctn{
    
    std::map<ctn::Tile, sf::Sprite> load_sprites(const YAML::Node& config, sf::Texture& texture);
    
}


#endif