#ifndef __WASTILS_OPERATOR_HPP
#define __WASTILS_OPERATOR_HPP


#include "SFML/Graphics.hpp"
#include <vector>


inline sf::Vector2f operator*(const sf::Vector2f lhs, const int& rhs){
    return sf::Vector2f(lhs.x * rhs, lhs.y * rhs);
}


template<typename T>
inline std::vector<T> operator+(const std::vector<T>& lhs, const std::vector<T>& rhs){
    std::vector<T> ret = lhs;
    ret.insert(ret.end(), rhs.begin(), rhs.end());

    return ret;
}


#endif