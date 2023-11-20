#include <numeric>
#include <sstream>
#include <iostream>
#include "polynomial.hpp"
#include "algorithm/algorithm.hpp"


namespace big
{
polynomial::polynomial(const std::map<std::size_t, rational> &coefficients)
{
	throw_if_empty(coefficients);

	coefficients_.resize(std::prev(coefficients.end())->first + 1);

	for (const auto &[degree, coefficient] : coefficients)
	{
		numeric::polynomial::coefficient_at(*this, degree) = coefficient;
	}

	erase_leading_zeroes();
}

std::ostream &operator<<(std::ostream &os, const polynomial &polynomial)
{
	if (numeric::is_zero(numeric::polynomial::degree(polynomial)))
	{
		return os << polynomial.major_coefficient();
	}

	for (std::size_t i = 0; i < std::ranges::size(polynomial.coefficients()); ++i)
	{
		const auto degree = numeric::polynomial::degree(polynomial) - i;
		const auto &coefficient = numeric::polynomial::coefficient_at(polynomial, degree);

                if (numeric::is_zero(coefficient))
		{
			continue;
		}

		bool is_one = numeric::abs(numeric::rational::numerator(coefficient)) == 1 && numeric::rational::denominator(coefficient) == 1;

		if (degree == numeric::polynomial::degree(polynomial) && is_one)
		{
			if (numeric::sign(coefficient) < 0)
			{
				os << '-';
			}
		}
		else
		{
			if (coefficient > 0 && degree != numeric::polynomial::degree(polynomial))
			{
				os << "+";
			}

			os << coefficient;
		}

		if (degree > 0 && !is_one)
		{
			os << "*";
		}

		if (degree > 0)
		{
			os << "x";

			if (degree > 1)
			{
				os << "^" << degree;
			}
		}
	}

	return os;
}
}
