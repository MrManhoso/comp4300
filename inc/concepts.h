#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>
#include <type_traits>

template <typename T>
concept Numerical = std::is_arithmetic_v<T>;

#endif