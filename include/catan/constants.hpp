#ifndef __CATAN_CONST_HPP
#define __CATAN_CONST_HPP

#include <map>
#include <string>
#include <vector>
#include <math.h>

namespace ctn{

    const double rad2deg{180.0 / M_PI};

    static const std::string
        DESERT{"desert"},
        WOOL{"wool"},
        ORE{"ore"},
        BRICK{"brick"},
        WHEAT{"wheat"},
        WOOD{"wood"},
        ANY{"any"};

    static const std::vector<std::string> tile_types = {
        ctn::DESERT,
        ctn::WOOL,
        ctn::ORE,
        ctn::BRICK,
        ctn::WHEAT,
        ctn::WOOD
    };

    static const std::string 
        HOUSE{"house"},
        HORIZONTAL{"horizontal"},
        LEFT_DIAGONAL{"left_diag"},
        RIGHT_DIAGONAL{"right_diag"},
        NONE{"none"};

    static const std::vector<std::string> path_types = {
        ctn::HORIZONTAL,
        ctn::LEFT_DIAGONAL,
        ctn::RIGHT_DIAGONAL
    };

    static const std::vector<std::string> house_types = {
        ctn::HOUSE
    };
    
    // Colors
    static const std::string 
        RED{"red"},
        BLUE{"blue"},
        YELLOW{"yellow"},
        GREEN{"green"};

    static const std::vector<std::string> colors = {
        ctn::RED,
        ctn::BLUE,
        ctn::YELLOW,
        ctn::GREEN
    };


    static const std::vector<std::string> materials = {
        ctn::WOOL,
        ctn::WOOD,
        ctn::ORE,
        ctn::BRICK,
        ctn::WHEAT
    };
}
#endif
