#include <numeric>
#include <sstream>
#include "polynomial.hpp"
#include "algorithm/algorithm.hpp"


namespace big
{
std::ostream &operator<<(std::ostream &os, const polynomial &polynomial)
{
	for (std::size_t i = 0; i < std::ranges::size(polynomial.coefficients()); ++i)
	{
		const auto &degree = numeric::polynomial::degree(polynomial);
		const auto &mapped_index = degree - i;
		const auto &coefficient = numeric::polynomial::coefficient_at(polynomial, mapped_index);

		if (i != degree && numeric::is_zero(coefficient))
		{
			continue;
		}

		if (i != 0 && !numeric::sign_bit(coefficient))
		{
			os << "+";
		}

		if (coefficient != 1 || i == degree)
		{
			os << coefficient;
		}

		if (i != degree)
		{
			if (coefficient != 1)
			{
				os << "*";
			}

			os << "x";

			if (mapped_index > 1)
			{
				os << "^" << mapped_index;
			}
		}
	}

	return os;
}
}
