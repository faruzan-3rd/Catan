#ifndef __CATAN_CONST_HPP
#define __CATAN_CONST_HPP

#include <map>
#include <string>

namespace ctn{

    enum class Tile{
        DESERT,
        PLAIN,
        WHEAT
    };

    static const ctn::Tile All_types[] = {
        ctn::Tile::DESERT,
        ctn::Tile::PLAIN,
        ctn::Tile::WHEAT
    };

    static const std::map<ctn::Tile, std::string> enum2str = {
        {ctn::Tile::DESERT, "desert"},
        {ctn::Tile::PLAIN, "plain"},
        {ctn::Tile::WHEAT, "wheat"}
    };
}


#endif