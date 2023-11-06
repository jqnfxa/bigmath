#pragma once

#include "../numeric/numeric.hpp"


namespace big::traits
{
template <typename T>
concept integral = requires (T t, T u)
{
	numeric::sign(t);
	numeric::abs(t);
};
}
