#pragma once

#include "../numeric/numeric.hpp"
#include "natural.hpp"


namespace big::traits
{
template <typename T>
concept integer_like = requires (T t)
{
	numeric::sign(t);
	numeric::abs(t);
};
}
