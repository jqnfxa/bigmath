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
}
