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

		// Normalize fraction
		reduce();
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

	const integer &rational::numerator() const & noexcept
	{
		return numerator_;
	}

	const natural &rational::denominator() const & noexcept
	{
		return denominator_;
	}

	rational rational::inverse() const &
	{
		if (numerator_.abs().is_zero())
		{
			throw std::logic_error("There is no multiplication inverse for zero");
		}

		return rational{integer(denominator_, !numerator_.is_positive()), numerator_.abs()};
	}

	rational &rational::operator+=(const rational &other) & noexcept
	{
		numerator_ = std::move(numerator_ * integer(other.denominator_) + integer(denominator_) * other.numerator_);
		denominator_ *= other.denominator_;

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

	bool rational::operator==(const rational &other) const & noexcept
	{
		return numerator_ == other.numerator_ && denominator_ == other.denominator_;
	}
}