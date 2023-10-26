#include <ranges>
#include <sstream>
#include "integer.hpp"

namespace big
{
	integer::integer(std::int64_t num) : integer(num < 0, natural(std::to_string(num)))
	{
	}

	integer::integer(bool is_negative, const natural &natural) : is_negative_(is_negative), absolute_value_(natural)
	{
	}

	integer::integer(bool is_negative, natural &&natural) : is_negative_(is_negative),
															absolute_value_(std::move(natural))
	{
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

	integer &integer::operator++() & noexcept
	{
		*this += integer(false, natural("1"));
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
		*this -= integer(false, natural("1"));
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
		if (!(is_negative_ ^ other.is_negative_))
		{
			absolute_value_ += other.absolute_value_;
		}
		else
		{
			// TODO optimize this idea
			if (is_negative_)
			{
				if (absolute_value_ > other.absolute_value_)
				{
					is_negative_ = true;
					absolute_value_ -= other.absolute_value_;
				}
				else
				{
					is_negative_ = false;
					absolute_value_ = std::move(other.absolute_value_ - absolute_value_);
				}
			}
			else
			{
				if (absolute_value_ > other.absolute_value_)
				{
					is_negative_ = false;
					absolute_value_ -= other.absolute_value_;
				}
				else
				{
					is_negative_ = true;
					absolute_value_ = std::move(other.absolute_value_ - absolute_value_);
				}
			}
		}

		return *this;
	}

	integer &integer::operator-=(const integer &other) &
	{
		// TODO optimize this idea
		if (!is_negative_ && !other.is_negative_)
		{
			if (*this < other)
			{
				is_negative_ = true;
				absolute_value_ = std::move(other.absolute_value_ - absolute_value_);
			}
			else
			{
				absolute_value_ -= other.absolute_value_;
			}
		}
		else if (!is_negative_ && other.is_negative_)
		{
			absolute_value_ += other.absolute_value_;
		}
		else if (is_negative_ && !other.is_negative_)
		{
			absolute_value_ += other.absolute_value_;
		}
		else if (is_negative_ && is_negative_)
		{
			if (*this < other)
			{
				absolute_value_ -= other.absolute_value_;
			}
			else
			{
				is_negative_ = false;
				absolute_value_ = std::move(other.absolute_value_ - absolute_value_);
			}
		}

		return *this;
	}

	integer &integer::operator*=(const integer &other) & noexcept
	{
		is_negative_ ^= other.is_negative_;
		absolute_value_ *= other.absolute_value_;
		return *this;
	}

	integer &integer::operator/=(const integer &other) &
	{
		is_negative_ ^= other.is_negative_;
		absolute_value_ /= other.absolute_value_;
		return *this;
	}

	integer &integer::operator<<=(std::size_t shift) &
	{
		absolute_value_ <<= shift;
		return *this;
	}

	integer &integer::operator>>=(std::size_t shift) & noexcept
	{
		absolute_value_ >>= shift;

		if (is_negative_)
		{
			is_negative_ = !is_zero();
		}
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
		return integer(false, std::move(absolute_value_ % other.absolute_value_));
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
}