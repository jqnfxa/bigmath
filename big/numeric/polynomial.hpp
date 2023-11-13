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
	{ t.degree() } -> std::same_as<std::size_t>;
};
}

template <typename T>
[[nodiscard]] constexpr std::size_t degree(const T &val) noexcept
{
	if constexpr (detail::member_degree<T>)
	{
		return val.degree();
	}
	else
	if constexpr (traits::rational_like<T>)
	{
		return 0;
	}
}

template <typename T>
[[nodiscard]] constexpr decltype(auto) coefficient_at(T &val, std::size_t pos) noexcept
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

template <typename T>
[[nodiscard]] constexpr decltype(auto) coefficient_at(const T &val, std::size_t pos) noexcept
{
	return std::as_const(coefficient_at(const_cast<T &>(val), pos));
}
}
