#pragma once

#include "../numeric/rational.hpp"


namespace big::traits
{
template <typename T>
concept rational_like = requires (T t)
{
	numeric::rational::numerator(t);
	numeric::rational::denominator(t);
};
}
