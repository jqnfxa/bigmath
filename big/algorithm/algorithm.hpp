#pragma once

#include <utility>
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"
#include "../numeric/rational.hpp"
#include "../traits/traits.hpp"


namespace big
{
/**
 * Calculates the greatest common divisor of two values.
 *
 * @tparam T Value type
 *
 * @param a Polynomial-like
 * @param b Polynomial-like
 *
 * @return Greatest common divisor of `a` and `b`
 *
 * @note GCF_NN_N | GCF_PP_P
 */
template <traits::polynomial_like T>
[[nodiscard]] constexpr T gcd(const T &a, const T &b) noexcept
{
	T first(a);
	T second(b);

	while (!numeric::is_zero(second))
	{
		first = std::exchange(second, first % second);
	}

	return first;
}

/**
 * Calculates the least common multiple of two values.
 *
 * @tparam T Value type
 *
 * @param a Polynomial-like
 * @param b Polynomial-like
 *
 * @return Least common multiple of `a` and `b`
 *
 * @note LCM_NN_N
 */
template <traits::polynomial_like T>
[[nodiscard]] constexpr T lcm(const T &a, const T &b)
{
	T result = a * b;
	result /= gcd(a, b);
	return result;
}

/**
 * Calculates the power of one value to the other.
 *
 * @tparam T Base type
 * @tparam U Exponent type
 *
 * @param base Power base
 * @param exp  Power exponent
 *
 * @return `base` raised to the power of `exp`
 */
template <traits::polynomial_like T, traits::integer_like U>
[[nodiscard]] constexpr T pow(T base, U exp) noexcept
{
	auto result = numeric::multiplicative_identity<T>();

	if (numeric::sign(exp) < 0)
	{
		return result / pow(base, numeric::abs(exp));
	}

	while (!numeric::is_zero(exp))
	{
		if (numeric::abs(exp).is_even())
		{
			base *= base;
			exp /= 2;
		}
		else
		{
			result *= base;
			--exp;
		}
	}

	return result;
}
}
