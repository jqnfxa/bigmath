#include "rational.hpp"
#include "algorithm/algorithm.hpp"

namespace big
{
	rational::rational() : rational(0, 1)
	{
	}

	rational::rational(const integer &num) noexcept: numerator_(num), denominator_(1)
	{
	}

	rational::rational(integer &&num) noexcept: numerator_(std::move(num)), denominator_(1)
	{
	}

	rational::rational(integer numerator, natural denominator) : numerator_(std::move(numerator)),
																 denominator_(std::move(denominator))
	{
		if (denominator_.is_zero())
		{
			throw std::invalid_argument("It is impossible to represent a fraction with denominator 0");
		}
	}

	void rational::reduce() & noexcept
	{
		auto coefficient = gcd(numerator_.abs(), denominator_);
		numerator_ /= integer(coefficient);
		denominator_ /= coefficient;
	}

	bool rational::is_integer() const & noexcept
	{
		// We must guarantee that function will call after reduce
		return denominator_ == 1;
	}

	rational rational::inverse() const &
	{
		if (numerator_.abs().is_zero())
		{
			throw std::domain_error("There is no multiplication inverse for zero");
		}

		return rational{integer(denominator_, !numerator_.is_positive()), numerator_.abs()};
	}

	rational &rational::operator+=(const rational &other) & noexcept
	{
		integer a(numerator_);
		integer b(denominator_);

		integer c(other.numerator_);
		integer d(other.denominator_);

		numerator_ = std::move(a * d + b * c);
		denominator_ = (b * d).abs();

		// Normalize fraction
		reduce();

		return *this;
	}

	rational &rational::operator-=(const rational &other) & noexcept
	{
		rational copy = other;
		copy.numerator_.flip_sing();

		*this += copy;

		return *this;
	}

	rational &rational::operator*=(const rational &other) & noexcept
	{
		numerator_ *= other.numerator_;
		denominator_ *= other.denominator_;

		// Normalize fraction
		reduce();

		return *this;
	}

	rational &rational::operator/=(const rational &other) &
	{
		*this *= other.inverse();
		return *this;
	}

	rational rational::operator+(const rational &other) const & noexcept
	{
		rational temp(*this);
		temp += other;
		return temp;
	}

	rational rational::operator-(const rational &other) const & noexcept
	{
		rational temp(*this);
		temp -= other;
		return temp;
	}

	rational rational::operator*(const rational &other) const & noexcept
	{
		rational temp(*this);
		temp *= other;
		return temp;
	}

	rational rational::operator/(const rational &other) const &
	{
		rational temp(*this);
		temp /= other;
		return temp;
	}
}