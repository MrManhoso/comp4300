#ifndef VEC2_H
#define VEC2_h

#include "inc/concepts.h"

#include "SFML/Graphics.hpp"

template <Numerical T>
class vec2
{
  public:
    T x;
    T y;

    vec2() = default;
    vec2(T xin, T yin) : x(xin), y(yin) {}
    vec2(const sf::Vector2<T>& vec) : x(vec.x), y(vec.y) {}
    vec2(const sf::Vector2i& vec) : x(vec.x), y(vec.y) {}
    
    operator sf::Vector2<T>() { return sf::Vector2<T>(x, y); }
    bool operator==(const vec2& rhs) {}
    bool operator!=(const vec2& rhs) {}
    vec2 operator+(const vec2& rhs) {}
    vec2 operator-(const vec2& rhs) {}
    vec2 operator*(const vec2& rhs) {}
    vec2 operator/(const vec2& rhs) {}
    void operator+=(const vec2& rhs) {}
    void operator-=(const vec2& rhs) {}
    void operator*=(const vec2& rhs) {}
    void operator/=(const vec2& rhs) {}
    
    void normalize() {}
    float length() {}
};

using vec2f = vec2<float>;

#endif