#include <ranges>
#include <sstream>
#include "integer.hpp"
#include "rational/rational.hpp"

namespace big
{
	integer::integer(const natural &natural, bool is_negative) noexcept: sign_bit_(is_negative), abs_(natural)
	{
	}

	integer::integer(natural &&natural, bool is_negative) noexcept: sign_bit_(is_negative), abs_(std::move(natural))
	{
	}

	integer::integer(const rational &other) noexcept: integer(0)
	{
		*this = std::move(other.numerator() / other.denominator());
		normalize();
	}

	bool integer::operator==(const integer &other) const & noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	std::strong_ordering integer::operator<=>(const integer &other) const & noexcept
	{
		if (sign_bit_ ^ other.sign_bit_)
		{
			if (sign_bit_ && !other.sign_bit_)
			{
				return std::strong_ordering::less;
			}

			return std::strong_ordering::greater;
		}

		if (sign_bit_ && other.sign_bit_)
		{
			// Since both nums have '-' then greater absolute value means less integer
			return other.abs_ <=> abs_;
		}

		return abs_ <=> other.abs_;
	}

	void integer::flip_sing() & noexcept
	{
		sign_bit_ = !sign_bit_;
	}

	bool integer::sign_bit() const & noexcept
	{
		return sign_bit_;
	}

	const natural &integer::abs() const & noexcept
	{
		return abs_;
	}

	integer integer::operator-() const & noexcept
	{
		return {abs(), !sign_bit_};
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
		if (sign_bit_ == other.sign_bit_)
		{
			abs_ += other.abs_;
		}
		else
		{
			flip_sing();
			*this = std::move(other - *this);
		}

		normalize();
		return *this;
	}

	integer &integer::operator+=(const natural &other) & noexcept
	{
		*this += integer(other);
		return *this;
	}

	integer &integer::operator-=(const integer &other) &
	{
		// If integers have different sing then we can just sum absolute values
		if (sign_bit_ ^ other.sign_bit_)
		{
			abs_ += other.abs_;
		}
		else
		{
			if (abs_ < other.abs_)
			{
				flip_sing();
				abs_ = std::move(other.abs_ - abs_);
			}
			else
			{
				abs_ = std::move(abs_ - other.abs_);
			}
		}

		normalize();
		return *this;
	}

	integer &integer::operator-=(const natural &other) &
	{
		*this -= integer(other);
		return *this;
	}

	integer &integer::operator*=(const integer &other) & noexcept
	{
		sign_bit_ ^= other.sign_bit_;
		*this *= other.abs();

		return *this;
	}

	integer &integer::operator*=(const natural &other) & noexcept
	{
		abs_ *= other;
		normalize();

		return *this;
	}

	integer &integer::operator/=(const integer &other) &
	{
		sign_bit_ ^= other.sign_bit_;
		*this /= other.abs();

		return *this;
	}

	integer &integer::operator/=(const natural &other) &
	{
		abs_ /= other;
		normalize();

		return *this;
	}

	integer &integer::operator%=(const integer &other) &
	{
		abs_ %= other.abs_;

		if (!is_zero() && sign_bit_ ^ other.sign_bit_)
		{
			*this += other;
		}

		normalize();
		return *this;
	}

	integer &integer::operator<<=(std::size_t shift) &
	{
		abs_ <<= shift;
		normalize();

		return *this;
	}

	integer &integer::operator>>=(std::size_t shift) & noexcept
	{
		abs_ >>= shift;
		normalize();

		return *this;
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
		return abs_.is_zero();
	}

	std::string integer::str() const & noexcept
	{
		std::ostringstream ss;
		ss << *this;
		return ss.str();
	}

	std::ostream &operator<<(std::ostream &out, const integer &num) noexcept
	{
		if (num.sign_bit())
		{
			out << '-';
		}
		return out << num.abs();
	}

	void integer::normalize() & noexcept
	{
		// Zero can't have sign
		if (is_zero())
		{
			sign_bit_ = false;
		}
	}
}
