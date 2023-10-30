#include <numeric>
#include "polynomial.hpp"
#include "algorithm/algorithm.hpp"

namespace big
{
	polynomial::polynomial() noexcept: coefficients_(1, rational{0, 1})
	{
	}

	polynomial::polynomial(const std::vector<rational> &coefficients) noexcept
	{
		coefficients_.assign(coefficients.rbegin(), coefficients.rend());
		shrink_to_fit();
	}

	void polynomial::shrink_to_fit() & noexcept
	{
		while (coefficients_.size() > 1 && coefficients_.back().numerator().is_zero())
		{
			coefficients_.pop_back();
		}
	}

	const rational &polynomial::major_coefficient() const & noexcept
	{
		return coefficients_.back();
	}

	const std::vector<rational> &polynomial::coefficients() const & noexcept
	{
		return coefficients_;
	}

	std::size_t polynomial::degree() const & noexcept
	{
		return coefficients_.size() - 1;
	}

	polynomial polynomial::derivative() const & noexcept
	{
		polynomial temp(*this);
		temp.coefficients_.front() = rational(0, 1);

		for (std::size_t i = 1; i < temp.coefficients_.size(); ++i)
		{
			temp.coefficients_[i - 1] = temp.coefficients_[i] * rational(i, 1);
		}

		if (temp.degree() > 0)
		{
			temp.coefficients_.pop_back();
		}

		return temp;
	}

	polynomial polynomial::fac() const & noexcept
	{
		polynomial temp(*this);

		integer numerators_gcd = temp.coefficients_.front().numerator();
		natural denominators_lcm = temp.coefficients_.front().denominator();

		for (auto &coefficient: temp.coefficients_)
		{
			numerators_gcd = integer(std::move(gcd(numerators_gcd.abs(), coefficient.numerator().abs())));
			denominators_lcm = std::move(gcd(denominators_lcm, coefficient.denominator()));
		}

		rational coeff(numerators_gcd, denominators_lcm);
		rational inv = coeff.inverse();

		for (auto &coefficient: temp.coefficients_)
		{
			coefficient *= inv;
		}

		return temp;
	}

	polynomial &polynomial::operator+=(const polynomial &other) & noexcept
	{
		coefficients_.resize(std::max(degree(), other.degree()) + 1, rational(0, 1));

		for (std::size_t i = 0; i < other.coefficients_.size(); ++i)
		{
			coefficients_[i] += other.coefficients_[i];
		}

		shrink_to_fit();
		return *this;
	}

	polynomial &polynomial::operator-=(const polynomial &other) & noexcept
	{
		coefficients_.resize(std::max(degree(), other.degree()) + 1, rational(0, 1));

		for (std::size_t i = 0; i < other.coefficients_.size(); ++i)
		{
			coefficients_[i] -= other.coefficients_[i];
		}

		shrink_to_fit();
		return *this;
	}

	polynomial &polynomial::operator*=(const rational &coefficient) & noexcept
	{
		for (auto &polynomial_coefficient: coefficients_)
		{
			polynomial_coefficient *= coefficient;
		}

		shrink_to_fit();
		return *this;
	}

	polynomial &polynomial::operator*=(const polynomial &other) & noexcept
	{
		std::vector<rational> result(degree() + other.degree() + 1, rational{0, 1});

		auto cur_len = coefficients_.size();
		auto other_len = other.coefficients_.size();

		for (size_t i = 0; i < cur_len; ++i)
		{
			for (size_t j = 0; j < other_len; ++j)
			{
				result[i + j] += coefficients_[i] * other.coefficients_[j];
			}
		}

		coefficients_ = std::move(result);
		shrink_to_fit();

		return *this;
	}

	polynomial &polynomial::operator<<=(std::size_t shift) &
	{
		if (coefficients_.size() > std::numeric_limits<std::size_t>::max() - shift)
		{
			throw std::length_error("Impossible to perform shift without losing data");
		}

		if (shift == 0)
		{
			return *this;
		}

		coefficients_.resize(coefficients_.size() + shift);

		// TODO iterator::difference_type (long) vs shift (std::size_t)
		auto old = std::next(coefficients_.rbegin(), shift);
		auto destination = coefficients_.rbegin();

		for (; old != coefficients_.rend(); ++destination, ++old)
		{
			*destination = *old;
		}
		for (; destination != coefficients_.rend(); ++destination)
		{
			*destination = rational{0, 1};
		}

		return *this;
	}

	polynomial &polynomial::operator/=(const polynomial &other) &
	{
		coefficients_ = std::move(divide_by(other).first.coefficients_);
		return *this;
	}

	polynomial &polynomial::operator%=(const polynomial &other) &
	{
		coefficients_ = std::move(divide_by(other).second.coefficients_);
		return *this;
	}

	polynomial polynomial::operator+(const polynomial &other) const & noexcept
	{
		polynomial temp(*this);
		temp += other;
		return temp;
	}

	polynomial polynomial::operator-(const polynomial &other) const & noexcept
	{
		polynomial temp(*this);
		temp -= other;
		return temp;
	}

	polynomial polynomial::operator*(const rational &coefficient) const & noexcept
	{
		polynomial temp(*this);
		temp *= coefficient;
		return temp;
	}

	polynomial polynomial::operator*(const polynomial &other) const & noexcept
	{
		polynomial temp(*this);
		temp *= other;
		return temp;
	}

	polynomial polynomial::operator<<(std::size_t shift) const &
	{
		polynomial temp(*this);
		temp <<= shift;
		return temp;
	}

	polynomial polynomial::operator/(const polynomial &other) const &
	{
		polynomial temp(*this);
		temp /= other;
		return temp;
	}

	polynomial polynomial::operator%(const polynomial &other) const &
	{
		polynomial temp(*this);
		temp %= other;
		return temp;
	}

	std::pair<polynomial, polynomial> polynomial::divide_by(const polynomial &other) &
	{
		if (other.degree() == 0 && other.major_coefficient().numerator().is_zero())
		{
			throw std::logic_error("Cannot divide by 0");
		}

		polynomial remainder = *this;
		polynomial divisor = other;
		polynomial quotient;

		while (remainder.degree() >= divisor.degree())
		{
			rational new_coefficient = remainder.major_coefficient() / divisor.major_coefficient();

			quotient <<= 1;
			quotient.coefficients_.front() = new_coefficient;

			remainder -= (divisor << (remainder.degree() - divisor.degree())) * new_coefficient;
		}

		quotient.shrink_to_fit();
		remainder.shrink_to_fit();

		return {quotient, remainder};
	}
}