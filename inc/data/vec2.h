#ifndef VEC2_H
#define VEC2_h

#include "inc/concepts.h"
#include <math.h>
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
    bool operator==(const vec2& rhs) { return x == rhs.x && y == rhs.y; }
    bool operator!=(const vec2& rhs) { return !(*this == rhs); }
    vec2 operator+(const vec2& rhs) { return vec2(x + rhs.x, y + rhs.y); }
    vec2 operator-(const vec2& rhs) { return vec2(x - rhs.x, y - rhs.y); }
    vec2 operator*(const T val) { return vec2(x * val, y * val); }
    vec2 operator/(const T val) { return vec2(x / val, y / val); }
    void operator+=(const vec2& rhs) 
    {
        x += rhs.x;
        y += rhs.y;
    }
    void operator-=(const vec2& rhs) 
    {
        x -= rhs.x;
        y -= rhs.y;
    }
    void operator*=(const T val)
    { 
        x *= val; 
        y *= val; 
    }
    void operator/=(const T val)
    { 
        x /= val; 
        y /= val; 
    }
    
    // Note! Speed is the length (magnitude) of the (velocity) vector
    inline float length() const { return std::sqrtf(x*x + y*y); }
    float dist(const vec2& rhs) const 
    { 
        auto dx = rhs.x - x;  
        auto dy = rhs.y - y;  
        return std::sqrtf(dx*dx, dy*dy);
    }

    void normalize() 
    {
        auto l = length();
        // TODO type conversion/rounding?
        x = x/L;
        y = y/L;
    }
};

using vec2f = vec2<float>;

#endif