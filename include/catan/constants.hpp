#ifndef __CATAN_CONST_HPP
#define __CATAN_CONST_HPP

#include <map>
#include <string>

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
}


#endif