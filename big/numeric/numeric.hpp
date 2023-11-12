#pragma once

#include <type_traits>
#include "../traits/natural.hpp"


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
}

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

template <typename T>
[[nodiscard]] constexpr signed char sign(const T &val) noexcept
{
	return sign_bit(val) ? -1 : 1;
}

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

// TODO: proper concepts
template <typename T>
[[nodiscard]] constexpr T multiplicative_identity() noexcept
{
	if constexpr (requires (T t) { { t.major_coefficient() }; })
	{
		return T{{1}};
	}
	else
	{
		return T{1};
	}
}

template <typename T, typename U>
[[nodiscard]] constexpr auto distance(const T &a, const U &b) noexcept
{
	const auto order = a <=> b;
	const auto a_abs = numeric::abs(a);
	const auto b_abs = numeric::abs(b);
	return order == std::strong_ordering::equal ? T{} : order > 0 ? a_abs - b_abs : b_abs - a_abs;
}
}
