#pragma once

#include "../numeric/numeric.hpp"


namespace big::traits
{
template <typename T>
concept integer_like = requires (T t)
{
	{ numeric::sign(t) } -> std::same_as<signed char>;
	{ numeric::abs(t) } -> std::common_reference_with<const natural &>;
};

namespace detail
{
template <typename T>
concept has_member_numerator = requires (T t)
{
	 { t.numerator() } -> integer_like;
};

template <typename T>
concept has_member_denominator = requires (T t)
{
	 { t.denominator() } -> std::common_reference_with<const natural &>;
};
}
}
