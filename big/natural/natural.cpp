#include <ranges>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <compare>
#include <algorithm>
#include "natural.hpp"

namespace big
{
	natural::natural(std::size_t num) noexcept
	{
		digits_.push_back(num);
	}

	natural::natural(const std::string &num) noexcept
	{
		// TODO implement
	}

	natural::natural(const natural::num_representation &num)
	{
		for (auto &item: num)
		{
			if (item >= numeral_system_base)
			{
				throw std::invalid_argument(
					std::to_string(item) + " cannot be represented in the " + std::to_string(numeral_system_base)
					+ " number system");
			}
		}

		digits_ = num;
		erase_leading_zeroes();
	}

	// TODO how to handle == and != in <=> operator?
	bool natural::operator==(const natural &other) const & noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	std::strong_ordering natural::operator<=>(const natural &other) const & noexcept
	{
		if (digits_.size() != other.digits_.size())
		{
			return digits_.size() <=> other.digits_.size();
		}

		const auto order = digits_.size() - 1;

		for (std::size_t i = 0; i <= order; ++i)
		{
			if (digits_[order - i] != other.digits_[order - i])
			{
				return digits_[order - i] <=> other.digits_[order - i];
			}
		}

		// If all digits are equal, the numbers are equal
		return std::strong_ordering::equal;
	}

	bool natural::is_even() const & noexcept
	{
		return digits_[0] % 2 == 0;
	}

	natural &natural::operator++() & noexcept
	{
		*this += 1;
		return *this;
	}

	natural natural::operator++(int) const & noexcept
	{
		natural temp(*this);
		++(temp);
		return temp;
	}

	natural &natural::operator--() & noexcept
	{
		*this -= 1;
		return *this;
	}

	natural natural::operator--(int) const & noexcept
	{
		natural temp(*this);
		--(temp);
		return temp;
	}

	natural &natural::operator+=(const natural &other) & noexcept
	{
		const auto max_order = std::max(digits_.size(), other.digits_.size());
		digits_.resize(max_order + 1);

		for (std::size_t i = 0; i < other.digits_.size(); ++i)
		{
			add_digit(other.digits_[i], i);
		}

		erase_leading_zeroes();
		return *this;
	}

	natural &natural::operator-=(const natural &other) &
	{
		if (other > *this)
		{
			throw std::domain_error("it is impossible to subtract a larger natural number");
		}

		for (std::size_t i = 0; i < other.digits_.size(); ++i)
		{
			subtract_digit(other.digits_[i], i);
		}

		erase_leading_zeroes();
		return *this;
	}

	// TODO stop here
	natural &natural::operator*=(const natural &other) & noexcept
	{
		if (is_zero() || other.is_zero())
		{
			// TODO how to assign zero value normally?
			nullify();
			return *this;
		}
		if (other == 1)
		{
			return *this;
		}

		num_representation result(num_.size() + other.num_.size(), 0);

		auto cur_num_len = num_.size();
		auto other_num_len = other.num_.size();

		for (std::size_t i = 0; i < cur_num_len; ++i)
		{
			for (std::size_t j = 0; j < other_num_len; ++j)
			{
				result[i + j] += num_[i] * other.num_[j];

				if (result[i + j] >= base)
				{
					result[i + j + 1] += result[i + j] / base;
				}

				result[i + j] %= base;
			}
		}

		num_ = std::move(result);
		erase_leading_zeroes();

		return *this;
	}

	natural &natural::operator/=(const natural &other) &
	{
		num_ = std::move(long_div(other).first.num_);
		return *this;
	}

	natural &natural::operator%=(const natural &other) &
	{
		num_ = std::move(long_div(other).second.num_);
		return *this;
	}

	natural &natural::operator<<=(std::size_t shift) &
	{
		if (num_.size() > std::numeric_limits<std::size_t>::max() - shift)
		{
			throw std::length_error("Impossible to perform shift without losing data");
		}

		if (is_zero() || shift == 0)
		{
			return *this;
		}

		num_.resize(num_.size() + shift);

		// TODO iterator::difference_type (long) vs shift (std::size_t)
		auto old = std::next(num_.rbegin(), shift);
		auto destination = num_.rbegin();

		for (; old != num_.rend(); ++destination, ++old)
		{
			*destination = *old;
		}
		for (; destination != num_.rend(); ++destination)
		{
			*destination = 0;
		}

		return *this;
	}

	natural &natural::operator>>=(std::size_t shift) & noexcept
	{
		if (is_zero() || shift == 0)
		{
			return *this;
		}
		if (shift >= num_.size())
		{
			nullify();
			return *this;
		}

		// TODO iterator::difference_type (long) vs shift (std::size_t)
		auto old = num_.begin() + shift;

		for (auto destination = num_.begin(); old != num_.end(); ++destination, ++old)
		{
			*destination = *old;
		}

		num_.resize(num_.size() - shift);

		return *this;
	}

	natural natural::operator+(const natural &other) const & noexcept
	{
		natural temp(*this);
		temp += other;
		return temp;
	}

	natural natural::operator-(const natural &other) const &
	{
		natural temp(*this);
		temp -= other;
		return temp;
	}

	natural natural::operator*(const natural &other) const & noexcept
	{
		natural temp(*this);
		temp *= other;
		return temp;
	}

	natural natural::operator/(const natural &other) const &
	{
		natural temp(*this);
		temp /= other;
		return temp;
	}

	natural natural::operator%(const natural &other) const &
	{
		natural temp(*this);
		temp %= other;
		return temp;
	}

	natural natural::operator<<(std::size_t shift) const &
	{
		natural temp(*this);
		temp <<= shift;
		return temp;
	}

	natural natural::operator>>(std::size_t shift) const & noexcept
	{
		natural temp(*this);
		temp >>= shift;
		return temp;
	}

	void natural::erase_leading_zeroes() & noexcept
	{
		while (num_.size() > 1 && num_.back() == 0)
		{
			num_.pop_back();
		}
	}

	void natural::nullify() & noexcept
	{
		if (num_.size() != 1)
		{
			num_.resize(1);
		}

		num_[0] = 0;
	}

	bool natural::is_zero() const & noexcept
	{
		return num_.size() == 1 && num_.front() == 0;
	}

	// TODO function is fast but not efficient (many copies)
	std::pair<natural, natural> natural::long_div(const natural &divisor) const &
	{
		if (divisor.is_zero())
		{
			throw std::domain_error("Cannot divide by zero");
		}
		if (*this < divisor)
		{
			return {0, *this};
		}

		natural quotient;
		natural remainder;
		uint8_t count;

		for (auto it: std::ranges::reverse_view(num_))
		{
			count = 0;

			remainder <<= 1;
			remainder += it;

			while (remainder >= divisor)
			{
				remainder -= divisor;
				++count;
			}

			if (count > 0 || quotient > 0)
			{
				quotient <<= 1;
				quotient += count;
			}
		}

		quotient.erase_leading_zeroes();
		remainder.erase_leading_zeroes();

		return {quotient, remainder};
	}

	std::string natural::to_str() const & noexcept
	{
		std::ostringstream ss;

		for (const auto &it: std::ranges::reverse_view(num_))
		{
			ss << static_cast<int16_t>(it);
		}

		return ss.str();
	}

	std::ostream &operator<<(std::ostream &out, const natural &num)
	{
		return out << num.to_str();
	}

	void natural::add_digit(cell_type digit, std::size_t position)
	{
		if (position > digits_.size())
		{
			throw std::out_of_range(std::to_string(position) + " is out of range");
		}
		if (position == digits_.size())
		{
			digits_.push_back(digit);
		}

		if (digits_[position] > numeral_system_base - digit)
		{
			digit -= numeral_system_base - digits_[position];
			digits_[position] = 0;
			add_digit(1, position + 1);
		}

		digits_[position] += digit;
	}

	void natural::subtract_digit(natural::cell_type digit, std::size_t position)
	{
		if (position >= digits_.size())
		{
			throw std::out_of_range(std::to_string(position) + " is out of range");
		}

		if (digits_[position] < digit)
		{
			auto borrow_position = position + 1;

			while (borrow_position < digits_.size() && digits_[borrow_position] == 0)
			{
				++borrow_position;
			}

			--digits_[borrow_position];

			for (std::size_t j = borrow_position - 1; j > position; --j)
			{
				digits_[j] += numeral_system_base - 1;
			}

			digits_[position] += numeral_system_base - digit;
		}
		else
		{
			digits_[position] -= digit;
		}
	}
}