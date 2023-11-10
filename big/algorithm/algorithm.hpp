#pragma once

#include <utility>
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"
#include "../numeric/rational.hpp"
#include "../numeric/rational.hpp"


namespace big
{
template <numeric::rational::algebraic T>
[[nodiscard]] constexpr T gcd(const T &a, const T &b) noexcept
{
	if (a < b)
	{
		return gcd(b, a);
	}

	T first = a;
	T second = b;

	while (!numeric::is_zero(second))
	{
		first = std::exchange(second, first % second);
	}

	return first;
}

template <numeric::rational::algebraic T>
[[nodiscard]] constexpr T lcm(const T &a, const T &b)
{
	T result = a * b;
	result /= gcd(a, b);
	return result;
}

template <numeric::rational::algebraic T>
[[nodiscard]] constexpr T pow(T object, integer power_base) noexcept
{
	T result = numeric::common_object(object);

	if (numeric::sign(power_base) < 0)
	{
		return result / pow(std::move(object), -power_base);
	}

	while (!numeric::is_zero(power_base))
	{
		if (numeric::abs(power_base).is_even())
		{
			object *= object;
			power_base /= 2;
		}
		else
		{
			result *= object;
			--power_base;
		}
	}

	return result;
}
}
