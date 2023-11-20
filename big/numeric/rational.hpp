#pragma once

#include "../numeric/numeric.hpp"
#include "../traits/integer.hpp"


namespace big::numeric::rational
{
namespace detail
{
template <typename T>
concept member_numerator = requires (T t)
{
	 { t.numerator() } -> traits::integer_like;
};

template <typename T>
concept member_denominator = requires (T t)
{
	 { t.denominator() } -> traits::natural_like;
};
}

/**
 * Retrieves the numerator of a number.
 *
 * @tparam T Value type
 *
 * @param val Rational-like
 *
 * @return Numerator of `val`
 *
 * @note TRANS_Q_Z
 */
template <typename T>
[[nodiscard]] constexpr decltype(auto) numerator(const T &val) noexcept
{
	if constexpr (detail::member_numerator<T>)
	{
		return val.numerator();
	}
	else
	if constexpr (traits::integer_like<T>)
	{
		return val;
	}
	else
	if constexpr (std::integral<T>)
	{
		return integer(val);
	}
}

/**
 * Retrieves the denominator of a number.
 *
 * @tparam T Value type
 *
 * @param val Rational-like
 *
 * @return Denominator of `val`
 */
template <typename T>
[[nodiscard]] constexpr decltype(auto) denominator(const T &val) noexcept
{
	if constexpr (detail::member_denominator<T>)
	{
		return val.denominator();
	}
	else
	if constexpr (traits::integer_like<T>)
	{
		return natural(1);
	}
}

/**
 * Checks given number for being an integer.
 *
 * @tparam T Value type
 *
 * @param val Rational-like
 *
 * @return `true` if denominator of `val` is
 *         the multiplicative identity of its type
 *
 * @note INT_Q_B
 */
template <typename T>
[[nodiscard]] constexpr bool is_integer(const T &val) noexcept
{
	return denominator(val) == multiplicative_identity<decltype(denominator(val))>();
}
}
