#pragma once

#include "../traits/natural.hpp"
#include <type_traits>


namespace big::numeric
{
namespace detail
{
template <typename T>
concept member_abs = requires (T t)
{
	{ t.abs() } -> traits::natural_like;
};

template <typename T>
concept member_sign_bit = requires (T t)
{
	{ t.sign_bit() } -> std::same_as<bool>;
};

template <typename T>
concept member_is_zero = requires (T t)
{
	{ t.is_zero() } -> std::same_as<bool>;
};

template <typename T>
concept member_degree = requires (T t)
{
	{ t.degree() } -> std::unsigned_integral;
};
}

/**
 * Retrieves the sign bit of a number.
 *
 * @tparam T Value type
 *
 * @param val Number
 *
 * @return Sign bit of `val`, converted to `bool`
 */
template <typename T>
[[nodiscard]] constexpr bool sign_bit(const T &val) noexcept
{
	if constexpr (traits::natural_like<T>)
	{
		return false;
	}
	else
	if constexpr (detail::member_sign_bit<T>)
	{
		return val.sign_bit();
	}
	else
	if constexpr (std::integral<T>)
	{
		return val < T{};
	}
}

/**
 * Retrieves the sign of a number.
 *
 * @tparam T Value type
 *
 * @param val Number
 *
 * @return `1` if `val` is nonnegative, `-1` otherwise
 */
template <typename T>
[[nodiscard]] constexpr signed char sign(const T &val) noexcept
{
	return sign_bit(val) ? -1 : 1;
}

/**
 * Retrieves the absolute value of a number.
 *
 * @tparam T Value type
 *
 * @param val Number
 *
 * @return Absolute value of `val`.
 *
 * @note The return type models `traits::natural_like`.
 *
 * @sa traits::natural_like
 */
template <typename T>
[[nodiscard]] constexpr decltype(auto) abs(const T &val) noexcept
{
	if constexpr (traits::natural_like<T>)
	{
		return val;
	}
	else
	if constexpr (detail::member_abs<T>)
	{
		return val.abs();
	}
	else
	if constexpr (std::integral<T>)
	{
		return natural(val);
	}
}

/**
 * Checks the number for being zero.
 *
 * @tparam T Value type
 *
 * @param val Number
 *
 * @return `true` if `val` is zero, `false` otherwise
 */
template <typename T>
[[nodiscard]] constexpr bool is_zero(const T &val) noexcept
{
	if constexpr (detail::member_is_zero<T>)
	{
		return val.is_zero();
	}
	else
	{
		return val == T{};
	}
}

/**
 * Constructs a multiplicative identity for the given type.
 *
 * @tparam T Value type
 *
 * @return Multiplicative identity of type `T`
 */
template <typename T>
[[nodiscard]] constexpr std::remove_cvref_t<T> multiplicative_identity() noexcept
{
	if constexpr (detail::member_degree<T>)
	{
		return T{{1}};
	}
	else
	{
		return T{1};
	}
}

/**
 * Calculates the distance between two values.
 *
 * @tparam T Value type
 *
 * @param a First value
 * @param b Second value
 *
 * @return `a - b` if `a > b`, `b - a` otherwise
 */
template <typename T>
[[nodiscard]] constexpr auto distance(const T &a, const T &b) noexcept
{
	const auto order = a <=> b;
	return order == std::strong_ordering::equal ? T{} : order > 0 ? a - b : b - a;
}
}
