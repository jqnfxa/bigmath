#include <numeric>
#include <sstream>
#include "polynomial.hpp"
#include "algorithm/algorithm.hpp"


namespace big
{
	polynomial polynomial::normalize() const &
	{
		polynomial temp(*this);
		temp.normalize();
		return temp;
	}

	void polynomial::normalize() &
	{
		auto scalar = numeric::polynomial::coefficient_at(*this, 0);

		for (auto &coefficient : coefficients_)
		{
			scalar *= numeric::sign(coefficient);
			scalar.numerator() = gcd(numeric::rational::numerator(scalar), numeric::rational::numerator(coefficient));
			scalar.denominator() = lcm(numeric::rational::denominator(scalar), numeric::rational::denominator(coefficient));
		}

		*this /= scalar;
	}

	polynomial polynomial::multiple_roots_to_simple() const &
	{
		polynomial temp(*this);
		temp /= gcd(temp, derivative());
		temp.normalize();
		return temp;
	}

	polynomial &polynomial::operator+=(const polynomial &other) & noexcept
	{
		coefficients_.resize(std::ranges::max(numeric::polynomial::degree(*this), numeric::polynomial::degree(other)) + 1);

		for (size_type i = 0; i < std::ranges::size(other.coefficients_); ++i)
		{
			numeric::polynomial::coefficient_at(*this, i) += numeric::polynomial::coefficient_at(other, i);
		}

		erase_leading_zeroes();
		return *this;
	}

	polynomial &polynomial::operator-=(const polynomial &other) & noexcept
	{
		*this *= -1;
		*this += other;
		*this *= -1;

		return *this;
	}

	polynomial &polynomial::operator*=(const polynomial &other) & noexcept
	{
		const auto &cur_len = std::ranges::size(coefficients_);
		const auto &other_len = std::ranges::size(other.coefficients_);

		std::vector<rational> result(cur_len + other_len - 1);

		for (size_type i = 0; i < cur_len; ++i)
		{
			for (size_type j = 0; j < other_len; ++j)
			{
				numeric::polynomial::coefficient_at(result, i + j) += numeric::polynomial::coefficient_at(*this, i) * numeric::polynomial::coefficient_at(other, j);
			}
		}

		coefficients_ = std::move(result);
		erase_leading_zeroes();

		return *this;
	}

	polynomial &polynomial::operator/=(const polynomial &other) &
	{
		*this = long_div(other).first;
		return *this;
	}

	polynomial &polynomial::operator%=(const polynomial &other) &
	{
		*this = long_div(other).second;
		return *this;
	}

	polynomial &polynomial::operator<<=(size_type shift) &
	{
		const auto &size = std::ranges::size(coefficients_);

		if (size > coefficients_.max_size() - shift)
		{
			throw std::length_error("impossible to perform shift without losing data");
		}
		if (shift == 0)
		{
			return *this;
		}

		coefficients_.resize(size + shift);

		const auto &rbegin = std::ranges::rbegin(coefficients_);
		std::copy(std::next(rbegin, shift), coefficients_.rend(), rbegin);
		std::fill_n(coefficients_.begin(), shift, rational{});

		return *this;
	}

	polynomial polynomial::operator+(const polynomial &other) const noexcept
	{
		polynomial temp(*this);
		temp += other;
		return temp;
	}

	polynomial polynomial::operator-(const polynomial &other) const noexcept
	{
		polynomial temp(*this);
		temp -= other;
		return temp;
	}

	polynomial polynomial::operator*(const polynomial &other) const noexcept
	{
		polynomial temp(*this);
		temp *= other;
		return temp;
	}

	polynomial polynomial::operator/(const polynomial &other) const
	{
		polynomial temp(*this);
		temp /= other;
		return temp;
	}

	polynomial polynomial::operator%(const polynomial &other) const
	{
		polynomial temp(*this);
		temp %= other;
		return temp;
	}

	polynomial polynomial::operator<<(size_type shift) const
	{
		polynomial temp(*this);
		temp <<= shift;
		return temp;
	}

	[[nodiscard]] std::string polynomial::str() const
	{
		std::stringstream ss;
                ss << *this;
                return ss.str();
	}

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

	std::pair<polynomial, polynomial> polynomial::long_div(const polynomial &divisor) const &
	{
		if (numeric::is_zero(numeric::polynomial::degree(divisor)) && numeric::is_zero(divisor.major_coefficient()))
		{
			throw std::logic_error("Cannot divide by 0");
		}
		if (numeric::polynomial::degree(divisor) > numeric::polynomial::degree(*this))
		{
			return {polynomial{}, *this};
		}

		polynomial remainder = *this;
		polynomial quotient{};

		quotient <<= numeric::polynomial::degree(remainder) - numeric::polynomial::degree(divisor);

		while (numeric::polynomial::degree(remainder) >= numeric::polynomial::degree(divisor) && !numeric::is_zero(remainder.major_coefficient()))
		{
			const auto &new_coefficient = remainder.major_coefficient() / divisor.major_coefficient();
			const auto &degree = numeric::polynomial::degree(remainder) - numeric::polynomial::degree(divisor);

			numeric::polynomial::coefficient_at(quotient, degree) = new_coefficient;

			polynomial subtract({new_coefficient});
			subtract <<= degree;
			subtract *= divisor;

			remainder -= subtract;
		}

		quotient.erase_leading_zeroes();
		remainder.erase_leading_zeroes();

		return {quotient, remainder};
	}
}
