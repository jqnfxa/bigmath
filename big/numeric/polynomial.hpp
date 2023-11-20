#pragma once

#include "../traits/rational.hpp"
#include <utility>


namespace big::numeric::polynomial
{
namespace detail
{
template <typename T>
concept member_coefficients = requires (T t)
{
	requires traits::rational_like<std::ranges::range_value_t<decltype(t.coefficients())>>;
};

template <typename T>
concept member_at = requires (T t, std::size_t pos)
{
	{ t.at(pos) } -> traits::rational_like;
};

template <typename T>
concept member_degree = requires (T t)
{
	{ t.degree() } -> std::unsigned_integral;
};
}

/**
 * Retrieves the degree of given polynomial.
 *
 * @tparam T Value type
 *
 * @param val Polynomial-like
 *
 * @return `val.degree()` if `val` has a member function `degree()`
 *         that returns an unsigned integral, `0` otherwise
 */
template <typename T>
[[nodiscard]] constexpr std::size_t degree(const T &val) noexcept
{
	if constexpr (detail::member_degree<T>)
	{
		return val.degree();
	}
	else
	{
		return 0;
	}
}

/**
 * Retrieves the coefficient at the given position of given polynomial.
 *
 * @tparam T Value type
 *
 * @param val Polynomial-like
 * @param pos Coefficient position
 *
 * @return Coefficient of `val` at `pos`
 *
 * @throws `std::out_of_range` if `pos` is greater than the degree of `val`
 */
template <typename T>
[[nodiscard]] constexpr decltype(auto) coefficient_at(T &val, std::size_t pos)
{
	if constexpr (detail::member_at<T>)
	{
		return val.at(pos);
	}
	else
	if constexpr (detail::member_coefficients<T>)
	{
		return val.coefficients()[pos];
	}
	else
	if constexpr (traits::rational_like<T>)
	{
		if (pos == 0)
		{
			return T{val};
		}

		return T{};
	}
}

/**
 * Retrieves the const-qualified coefficient at the given position of given polynomial.
 *
 * @tparam T Value type
 *
 * @param val Polynomial-like
 * @param pos Coefficient position
 *
 * @return Coefficient of `val` at `pos`
 *
 * @throws `std::out_of_range` if `pos` is greater than the degree of `val`
 */
template <typename T>
[[nodiscard]] constexpr decltype(auto) coefficient_at(const T &val, std::size_t pos) noexcept
{
	return std::as_const(coefficient_at(const_cast<T &>(val), pos));
}
}
