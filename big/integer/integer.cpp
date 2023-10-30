#include <ranges>
#include <sstream>
#include "integer.hpp"
#include "rational/rational.hpp"

namespace big
{
	integer::integer(std::int64_t num) noexcept: integer(std::abs(num), num < 0)
	{
	}

	integer::integer(const natural &natural, bool is_negative) noexcept: absolute_value_(natural),
																		 is_negative_(is_negative)
	{
	}

	integer::integer(natural &&natural, bool is_negative) noexcept: absolute_value_(std::move(natural)),
																	is_negative_(is_negative)
	{
	}

	integer::integer(const rational &other) noexcept
	{
		is_negative_ = other.numerator().is_negative_;
		absolute_value_ = std::move(other.numerator().abs() / other.denominator());
		normalize();
	}

	bool integer::operator==(const integer &other) const & noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	std::strong_ordering integer::operator<=>(const integer &other) const & noexcept
	{
		if (is_negative_ ^ other.is_negative_)
		{
			if (is_negative_ && !other.is_negative_)
			{
				return std::strong_ordering::less;
			}

			return std::strong_ordering::greater;
		}

		if (is_negative_ && other.is_negative_)
		{
			// Since both nums have '-' then greater absolute value means less integer
			return other.absolute_value_ <=> absolute_value_;
		}

		return absolute_value_ <=> other.absolute_value_;
	}

	void integer::flip_sing() & noexcept
	{
		is_negative_ = !is_negative_;
	}

	bool integer::is_positive() const & noexcept
	{
		return !is_negative_;
	}

	const natural &integer::abs() const & noexcept
	{
		return absolute_value_;
	}

	integer integer::operator-() const & noexcept
	{
		integer temp(*this);
		temp.flip_sing();
		return temp;
	}

	integer integer::operator+() const & noexcept
	{
		return {*this};
	}

	integer &integer::operator++() & noexcept
	{
		*this += 1;
		return *this;
	}

	integer integer::operator++(int) const & noexcept
	{
		integer temp(*this);
		++(temp);
		return temp;
	}

	integer &integer::operator--() & noexcept
	{
		*this -= 1;
		return *this;
	}

	integer integer::operator--(int) const & noexcept
	{
		integer temp(*this);
		--(temp);
		return temp;
	}

	integer &integer::operator+=(const integer &other) & noexcept
	{
		// If integers have same sign then just sum absolute values
		if (is_negative_ == other.is_negative_)
		{
			absolute_value_ += other.absolute_value_;
		}
		else
		{
			flip_sing();
			*this = std::move(other - *this);
		}

		normalize();
		return *this;
	}

	integer &integer::operator-=(const integer &other) &
	{
		// If integers have different sing then we can just sum absolute values
		if (is_negative_ ^ other.is_negative_)
		{
			absolute_value_ += other.absolute_value_;
		}
		else
		{
			if (absolute_value_ < other.absolute_value_)
			{
				flip_sing();
				absolute_value_ = std::move(other.absolute_value_ - absolute_value_);
			}
			else
			{
				absolute_value_ = std::move(absolute_value_ - other.absolute_value_);
			}
		}

		normalize();
		return *this;
	}

	integer &integer::operator*=(const integer &other) & noexcept
	{
		is_negative_ ^= other.is_negative_;
		absolute_value_ *= other.absolute_value_;

		normalize();
		return *this;
	}

	integer &integer::operator/=(const integer &other) &
	{
		is_negative_ ^= other.is_negative_;
		absolute_value_ /= other.absolute_value_;

		normalize();
		return *this;
	}

	integer &integer::operator%=(const integer &other) &
	{
		absolute_value_ %= other.absolute_value_;

		if (!is_zero() && is_negative_ ^ other.is_negative_)
		{
			*this += other;
		}

		normalize();
		return *this;
	}

	integer &integer::operator<<=(std::size_t shift) &
	{
		absolute_value_ <<= shift;

		normalize();
		return *this;
	}

	integer &integer::operator>>=(std::size_t shift) & noexcept
	{
		absolute_value_ >>= shift;

		normalize();
		return *this;
	}

	integer integer::operator+(const integer &other) const & noexcept
	{
		integer temp(*this);
		temp += other;
		return temp;
	}

	integer integer::operator-(const integer &other) const &
	{
		integer temp(*this);
		temp -= other;
		return temp;
	}

	integer integer::operator*(const integer &other) const & noexcept
	{
		integer temp(*this);
		temp *= other;
		return temp;
	}

	integer integer::operator/(const integer &other) const &
	{
		integer temp(*this);
		temp /= other;
		return temp;
	}

	integer integer::operator%(const integer &other) const &
	{
		integer temp(*this);
		temp %= other;
		return temp;
	}

	integer integer::operator<<(std::size_t shift) const &
	{
		integer temp(*this);
		temp <<= shift;
		return temp;
	}

	integer integer::operator>>(std::size_t shift) const & noexcept
	{
		integer temp(*this);
		temp >>= shift;
		return temp;
	}

	bool integer::is_zero() const & noexcept
	{
		return absolute_value_.is_zero();
	}

	std::string integer::to_str() const & noexcept
	{
		std::ostringstream ss;

		if (is_negative_)
		{
			ss << '-';
		}

		ss << absolute_value_.to_str();

		return ss.str();
	}

	std::ostream &operator<<(std::ostream &out, const integer &num)
	{
		return out << num.to_str();
	}

	void integer::normalize() & noexcept
	{
		// Zero can't have sign
		if (is_zero())
		{
			is_negative_ = false;
		}
	}
}