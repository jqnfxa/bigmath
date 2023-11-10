#pragma once

#include "../traits/traits.hpp"
#include "../integer/integer.hpp"


namespace big::numeric::rational
{
template <typename T>
[[nodiscard]] constexpr decltype (auto) numerator(const T &val) noexcept
{
	if constexpr (traits::detail::has_member_numerator<T>)
	{
		return val.numerator();
	}
	if constexpr (std::integral<T>)
	{
		return integer(val);
	}
	if constexpr (!std::integral<T> && traits::integer_like<T>)
	{
		return val;
	}
}

template <typename T>
[[nodiscard]] constexpr decltype (auto) denominator(const T &val) noexcept
{
	if constexpr (traits::detail::has_member_denominator<T>)
	{
		return val.denominator();
	}
	if constexpr (std::integral<T> || traits::integer_like<T>)
	{
		return natural(1);
	}
}

template <typename T>
concept rationalisable = requires (T t)
{
	numerator(t);
	denominator(t);
};

template <typename T>
concept algebraic = rationalisable<T> || numeric::detail::member_has_x<T>;
}
