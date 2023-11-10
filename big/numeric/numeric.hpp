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
	{ t.abs() } -> std::common_reference_with<const natural &>;
};

template <typename T>
concept member_sign_bit = requires (T t)
{
	{ t.sign_bit() } -> std::same_as<bool>;
};

template <typename T>
concept member_has_x = requires (T t)
{
	{ t.major_coefficient().sign_bit() } -> std::same_as<bool>;
};
}

template <typename T>
[[nodiscard]] constexpr bool sign_bit(const T &val) noexcept
{
	if constexpr (std::same_as<T, natural>)
	{
		return false;
	}
	if constexpr (detail::member_sign_bit<T>)
	{
		return val.sign_bit();
	}
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
	if constexpr (detail::member_abs<T>)
	{
		return val.abs();
	}
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

template <typename T>
[[nodiscard]] constexpr bool is_zero(const T &val) noexcept
{
	if constexpr (detail::member_has_x<T> || std::same_as<T, natural> || detail::member_sign_bit<T>)
	{
		return val.is_zero();
	}
	if constexpr (std::integral<T>)
	{
		return val == 0;
	}
}

template <typename T>
[[nodiscard]] constexpr T common_object(const T &val) noexcept
{
	if constexpr (numeric::detail::member_has_x<T>)
	{
		return T{{1}};
	}
	else 
	{
		return T{1};
	}
}
}
