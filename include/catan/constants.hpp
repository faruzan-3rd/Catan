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

    static const std::vector<std::string> materials = {
        ctn::WOOL,
        ctn::WOOD,
        ctn::ORE,
        ctn::BRICK,
        ctn::WHEAT
    };

    // Buildings
    static const std::string 
        HOUSE{"house"},
        CITY{"city"},
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
        GREEN{"green"},
        GRAY{"gray"},
        INVISIBLE{"invisible"};

    static const std::vector<std::string> colors = {
        ctn::RED,
        ctn::BLUE,
        ctn::YELLOW,
        ctn::GREEN,
        ctn::GRAY,
        ctn::INVISIBLE
    };


    // Cards
    static const std::string 
        KNIGHT{"knight"},
        ROAD_BUILD{"road_building"},
        YoPLENTY{"year_of_plenty"},
        MONOPOLY{"monopoly"},
        GREAT_HALL{"great_hall"};

    static const std::string
        LARGEST_ARMY{"largest_army"},
        LONGEST_ROAD{"longest_road"};

    static const std::vector<std::string> dev_cards = {
        ctn::KNIGHT,
        ctn::ROAD_BUILD,
        ctn::YoPLENTY,
        ctn::MONOPOLY,
        ctn::GREAT_HALL
    };

    static const std::vector<std::string> special_cards = {
        ctn::LARGEST_ARMY,
        ctn::LONGEST_ROAD
    };

    // Phase identifiers
    static const int
        SETUP{1},
        RESOURCE{2},
        TRADE{3},
        BUILD{4};

    enum class SelectionType{
        Settlement,
        Path,
        Card,
        Tile,
        None
    };
    static const int
        SETTLEMENT{0b0001},
        PATH{0b0010},
        CARD{0b0100},
        TILE{0b1000};

    // Dice stuff
    static const std::string
        ONE{"one"},
        TWO{"two"},
        THREE{"three"},
        FOUR{"four"},
        FIVE{"five"},
        SIX{"six"};

    static const std::vector<std::string> dice_faces = {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX
    };

    // Other
    static const std::string
        HARBOR{"harbor"},
        TOKEN{"token"},
        SEA{"sea"},
        ROBBER{"robber"},
        TILE_PREVIEW{"tile_preview"};

    enum class PlayerType{
        AI,
        HUMAN
    };

    enum class UIButton{
        Build,
        DiceRoll,
        Confirm_dices,
        Confirm_tile
    };
}
#endif
