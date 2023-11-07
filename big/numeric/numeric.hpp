#pragma once

#include <type_traits>
#include "../natural/natural.hpp"


namespace big::numeric
{
namespace detail
{
template <typename T>
concept member_abs = requires (T t)
{
	{ t.abs() } -> std::same_as<const natural &>;
};

template <typename T>
concept member_sign_bit = requires (T t)
{
	{ t.sign_bit() } -> std::same_as<bool>;
};
}

template <typename T>
[[nodiscard]] constexpr bool sign_bit(const T &val) noexcept
{
	if constexpr (std::same_as<T, natural>)
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
	if constexpr (std::same_as<T, natural>)
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

template <typename T, typename U>
[[nodiscard]] constexpr auto distance(const T &a, const U &b) noexcept
{
	const auto order = a <=> b;
	return order == std::strong_ordering::equal ? 0 : order > 0 ? a - b : b - a;
}
}
