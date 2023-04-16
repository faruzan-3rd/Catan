#ifndef __CATAN_CONST_HPP
#define __CATAN_CONST_HPP

#include <map>
#include <string>
#include <vector>
#include <math.h>

namespace ctn{

    const double rad2deg{180.0 / M_PI};

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

    static const std::vector<std::string> path_types = {
        ctn::HORIZONTAL,
        ctn::LEFT_DIAGONAL,
        ctn::RIGHT_DIAGONAL
    };

    static const std::vector<std::string> house_types = {
        ctn::HOUSE
    };
    
    
    static const std::string RED{"red"};
    static const std::string BLUE{"blue"};
    static const std::string YELLOW{"yellow"};
    static const std::string GREEN{"green"};

    static const std::vector<std::string> colors = {
        ctn::RED,
        ctn::BLUE,
        ctn::YELLOW,
        ctn::GREEN
    };

    static const std::string mWOOL{"wool"};
    static const std::string mORE{"ore"};
    static const std::string mBRICK{"brick"};
    static const std::string mWHEAT{"wheat"};
    static const std::string mWOOD{"wood"};
    static const std::string mANY{"any"};

    static const std::vector<std::string> materials = {
        ctn::mWOOL,
        ctn::mWOOD,
        ctn::mORE,
        ctn::mBRICK,
        ctn::mWHEAT
    };

}
#endif
