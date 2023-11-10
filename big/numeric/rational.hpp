#pragma once

#include "../traits/traits.hpp"


namespace big::numeric::rational
{
template <typename T>
[[nodiscard]] constexpr decltype (auto) numerator(const T &val) noexcept
{
	if constexpr (traits::detail::has_member_numerator<T>)
	{
		return val.numerator();
	}
	if constexpr (traits::integer_like<T>)
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
	if constexpr (traits::integer_like<T>)
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
}
