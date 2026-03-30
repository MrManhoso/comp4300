#ifndef VEC2_H
#define VEC2_h

#include "inc/concepts.h"

template <Numerical T>
class vec2
{
  public:
    T x;
    T y;
    
    bool operator==(const vec2<T>& other);
    bool operator!=(const vec2<T>& other);
    bool operator+(const vec2<T>& other);
    bool operator-(const vec2<T>& other);
    bool operator*(const vec2<T>& other);
    bool operator/(const vec2<T>& other);
    
    void normalize();
    float length();
};

using vec2f = vec2<float>;

#endif