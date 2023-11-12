#include <numeric>
#include <sstream>
#include "polynomial.hpp"
#include "algorithm/algorithm.hpp"

namespace big
{
	polynomial::polynomial() noexcept
		: coefficients_(1, rational{})
	{
	}

	polynomial::polynomial(const std::vector<rational> &coefficients)
	{
		if (std::ranges::empty(coefficients))
		{
			throw std::invalid_argument("coefficients cannot be empty");
		}

		coefficients_.assign(std::ranges::rbegin(coefficients), std::ranges::rend(coefficients));
		shrink_to_fit();
	}

	void polynomial::shrink_to_fit() & noexcept
	{
		while (std::ranges::size(coefficients_) > 1 && numeric::rational::numerator(major_coefficient()).is_zero())
		{
			coefficients_.pop_back();
		}
	}

	polynomial polynomial::derivative() const & noexcept
	{
		polynomial temp(*this);
		temp.coefficients_.at(0) = rational{};

		for (size_type i = 1; i < std::ranges::size(temp.coefficients_); ++i)
		{
			temp.coefficients_[i - 1] = temp.coefficients_[i] * i;
		}
		if (temp.degree() > 0)
		{
			temp.coefficients_.pop_back();
		}

		return temp;
	}

	polynomial polynomial::normalize() const & noexcept
	{
		polynomial temp(*this);
		temp.normalize();
		return temp;
	}

	void polynomial::normalize() & noexcept
	{
		rational scalar = coefficients_.at(0);
		bool sign = numeric::sign_bit(scalar);

		for (auto &coefficient : coefficients_)
		{
			sign ^= numeric::sign_bit(coefficient);
			scalar.numerator() = gcd(numeric::abs(numeric::rational::numerator(scalar)), 
						numeric::abs(numeric::rational::numerator(coefficient)));
			scalar.denominator() = lcm(scalar.denominator(), coefficient.denominator());
		}

		if (numeric::sign_bit(scalar) ^ sign)
		{
			scalar.flip_sign();
		}

		*this /= scalar;
	}

	polynomial polynomial::multiple_roots_to_simple() const & noexcept
	{
		polynomial temp(*this);
		temp /= gcd(temp, derivative());
		temp.normalize();
		return temp;
	}

	rational &polynomial::at(std::size_t degree) &
	{
		return const_cast<rational &>(const_cast<const polynomial *>(this)->at(degree));
	}

	const rational &polynomial::at(std::size_t degree) const &
	{
		if (degree >= coefficients_.size())
		{
			throw std::out_of_range("degree is out of range");
		}

		return coefficients_[degree];
	}

	rational &polynomial::operator[](std::size_t degree) &
	{
		return at(degree);
	}

	const rational &polynomial::operator[](std::size_t degree) const &
	{
		return at(degree);
	}

	polynomial &polynomial::operator+=(const polynomial &other) & noexcept
	{
		coefficients_.resize(std::ranges::max(degree(), other.degree()) + 1, rational{});

		for (size_type i = 0; i < other.coefficients_.size(); ++i)
		{
			coefficients_[i] += other.coefficients_[i];
		}

		shrink_to_fit();
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
		std::vector<rational> result(degree() + other.degree() + 1, rational{});

		const auto &cur_len = coefficients_.size();
		const auto &other_len = other.coefficients_.size();

		for (size_type i = 0; i < cur_len; ++i)
		{
			for (size_type j = 0; j < other_len; ++j)
			{
				result[i + j] += coefficients_[i] * other.coefficients_[j];
			}
		}

		coefficients_ = std::move(result);
		shrink_to_fit();

		return *this;
	}

	polynomial &polynomial::operator/=(const polynomial &other) &
	{
		coefficients_ = long_div(other).first.coefficients_;
		return *this;
	}

	polynomial &polynomial::operator%=(const polynomial &other) &
	{
		coefficients_ = long_div(other).second.coefficients_;
		return *this;
	}

	polynomial &polynomial::operator<<=(size_type shift) &
	{
		const auto &size = coefficients_.size();

		if (size > coefficients_.max_size() - shift)
		{
			throw std::length_error("impossible to perform shift without losing data");
		}
		if (shift == 0)
		{
			return *this;
		}

		coefficients_.resize(size + shift);

		const auto &rbegin = coefficients_.rbegin();
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

	polynomial polynomial::operator<<(std::size_t shift) const
	{
		polynomial temp(*this);
		temp <<= shift;
		return temp;
	}

	[[nodiscard]] std::string polynomial::to_str() const
	{
		std::stringstream ss;
                ss << *this;
                return ss.str();
	}

	std::ostream &operator<<(std::ostream &os, const polynomial &polynomial)
	{
		for (polynomial::size_type i = 0; i < polynomial.coefficients().size(); ++i)
		{
			const auto &mapped_index = polynomial.degree() - i;

			if (numeric::is_zero(polynomial[mapped_index]) && i != polynomial.degree())
			{
				continue;
			}

			if (i != 0 && !numeric::sign_bit(polynomial[mapped_index]))
			{
				os << "+";
			}

			if (polynomial[mapped_index] != 1 || i == polynomial.degree())
			{
				os << polynomial[mapped_index];
			}

			if (i != polynomial.degree())
			{
				if (polynomial[mapped_index] != 1)
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
		if (divisor.degree() == 0 && numeric::rational::numerator(divisor.major_coefficient()).is_zero())
		{
			throw std::logic_error("Cannot divide by 0");
		}
		if (divisor.degree() > degree())
		{
			return {polynomial{}, *this};
		}

		polynomial remainder = *this;
		polynomial quotient{};

		quotient <<= remainder.degree() - divisor.degree();

		while (remainder.degree() >= divisor.degree() && !numeric::rational::numerator(remainder.major_coefficient()).is_zero())
		{
			const auto new_coefficient = remainder.major_coefficient() / divisor.major_coefficient();
			const auto degree = remainder.degree() - divisor.degree();

			quotient[degree] = new_coefficient;

			polynomial subtract({new_coefficient});
			subtract <<= degree;
			subtract *= divisor;

			remainder -= subtract;
		}

		quotient.shrink_to_fit();
		remainder.shrink_to_fit();

		return {quotient, remainder};
	}
}
