#pragma once

#include <type_traits>
#include "numeric.hpp"
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"


namespace big::numeric::rational
{
namespace detail
{
template <typename T>
concept has_member_numerator = requires (T t)
{
	 { t.numerator() } -> std::common_reference_with<const integer&>;
};

template <typename T>
concept has_member_denominator = requires (T t)
{
	 { t.denominator() } -> std::common_reference_with<const natural&>;
};
}

template <typename T>
[[nodiscard]] constexpr decltype (auto) numerator(const T &val) noexcept
{
	if constexpr (detail::has_member_numerator<T>)
	{
		return val.numerator();
	}
	if constexpr (std::integral<T>)
	{
		return val;
	}
}

template <typename T>
[[nodiscard]] constexpr decltype (auto) denominator(const T &val) noexcept
{
	if constexpr (detail::has_member_denominator<T>)
	{
		return val.denominator();
	}
	if constexpr (std::integral<T>)
	{
		return natural(1);
	}
}
}
