#include "wastils/geometry.hpp"


double was::distance_sq(const sf::Vector2f& u, const sf::Vector2f& v){
    float dx = u.x - v.x;
    float dy = u.y - v.y;

    return dx * dx + dy * dy;
}