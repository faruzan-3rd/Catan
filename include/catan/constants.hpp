#ifndef __CATAN_CONST_HPP
#define __CATAN_CONST_HPP

#include <map>
#include <string>
#include <vector>

namespace ctn{

    enum class Tile{
        DESERT,
        PLAIN,
        WHEAT,
        MOUNTAIN,
        BRICK,
        FOREST
    };

    static const ctn::Tile All_types[] = {
        ctn::Tile::DESERT,
        ctn::Tile::PLAIN,
        ctn::Tile::WHEAT,
        ctn::Tile::MOUNTAIN,
        ctn::Tile::BRICK,
        ctn::Tile::FOREST
    };

    static const std::map<ctn::Tile, std::string> enum2str = {
        {ctn::Tile::DESERT, "desert"},
        {ctn::Tile::PLAIN, "plain"},
        {ctn::Tile::WHEAT, "wheat"},
        {ctn::Tile::MOUNTAIN, "mountain"},
        {ctn::Tile::BRICK, "brick"},
        {ctn::Tile::FOREST, "forest"}
    };

    static const std::string HOUSE{"house"};
    static const std::string HORIZONTAL{"horizontal"};
    static const std::string LEFT_DIAGONAL{"left_diag"};
    static const std::string RIGHT_DIAGONAL{"right_diag"};

    static const std::vector<std::string> building_types = {
        ctn::HOUSE,
        ctn::HORIZONTAL,
        ctn::LEFT_DIAGONAL,
        ctn::RIGHT_DIAGONAL
    };
}


#endif