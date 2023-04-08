#ifndef __WASTILS_OPERATOR_HPP
#define __WASTILS_OPERATOR_HPP


#include "SFML/Graphics.hpp"


inline sf::Vector2f operator*(const sf::Vector2f lhs, const int& rhs){
    return sf::Vector2f(lhs.x * rhs, lhs.y * rhs);
}



#endif