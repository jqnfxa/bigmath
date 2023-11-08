#pragma once

#include "../numeric/numeric.hpp"


namespace big::traits
{
class rational;

template <typename T>
concept integral = requires (T t, T u)
{
	numeric::sign(t);
	numeric::abs(t);
};

template <typename T>
concept unsigned_integral = std::integral<T> && (numeric::sign(T{}) > 0);

template <typename T>
concept rationalisable = integral<T> || std::is_same_v<T, rational>;
}
