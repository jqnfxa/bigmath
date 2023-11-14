#pragma once

#include "../numeric/polynomial.hpp"


namespace big::traits
{
template <typename T>
concept polynomial_like = requires (T t, std::size_t pos)
{
	numeric::polynomial::coefficient_at(t, pos);
	numeric::polynomial::degree(t);
};
}
