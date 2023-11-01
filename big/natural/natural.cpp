#include <ranges>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <compare>
#include <algorithm>
#include "natural.hpp"

namespace big
{
	natural::natural(std::uintmax_t num) noexcept
	{
		do
		{
			digits_.push_back(num % numeral_system_base);
			num /= numeral_system_base;
		} while (num != 0);

		erase_leading_zeroes();
	}

	natural::natural(const std::string &num) noexcept
	{
		// TODO implement
	}

	natural::natural(const num_representation &num)
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

		const auto &order = digits_.size() - 1;

		for (size_type i = 0; i <= order; ++i)
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
		return digits_[0] & 1;
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
		digits_.resize(std::max(digits_.size(), other.digits_.size()) + 1);

		for (size_type i = 0; i < other.digits_.size(); ++i)
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

	natural &natural::operator*=(const natural &other) & noexcept
	{
		if (is_zero() || other.is_zero())
		{
			nullify();
			return *this;
		}
		if (other == 1)
		{
			return *this;
		}

		const auto this_len = digits_.size();
		const auto other_len = other.digits_.size();

		num_representation result(this_len + other_len, 0);

		for (size_type i = 0; i < this_len; ++i)
		{
			for (size_type j = 0; j < other_len; ++j)
			{
				std::uintmax_t product = digits_[i];
				product *= other.digits_[j];
				product += digits_[i + j];

				if (product >= numeral_system_base)
				{
					add_digit(product / numeral_system_base, i + j + 1);
				}

				digits_[i + j] = product % numeral_system_base;
			}
		}

		digits_ = std::move(result);
		erase_leading_zeroes();

		return *this;
	}

	natural &natural::operator/=(const natural &other) &
	{
		digits_ = std::move(long_div(other).first.digits_);
		return *this;
	}

	natural &natural::operator%=(const natural &other) &
	{
		digits_ = std::move(long_div(other).second.digits_);
		return *this;
	}

	natural &natural::operator<<=(std::size_t shift) &
	{
		const auto &size = digits_.size();

		if (size > digits_.max_size() - shift)
		{
			throw std::length_error("impossible to perform shift without losing data");
		}
		if (is_zero() || shift == 0)
		{
			return *this;
		}

		digits_.resize(size + shift);

		const auto rbegin = digits_.rbegin();
		std::copy(std::next(rbegin, shift), digits_.rend(), rbegin);
		std::fill_n(digits_.begin(), shift, 0);

		return *this;
	}

	natural &natural::operator>>=(std::size_t shift) & noexcept
	{
		if (is_zero() || shift == 0 || shift >= digits_.size())
		{
			nullify();
			return *this;
		}

		const auto &size = digits_.size();
		const auto begin = digits_.begin();

		digits_.erase(begin, std::next(begin, shift));
		digits_.resize(size - shift);

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
		while (digits_.size() > 1 && digits_.back() == 0)
		{
			digits_.pop_back();
		}
		if (digits_.empty())
		{
			nullify();
		}
	}

	void natural::nullify() & noexcept
	{
		if (digits_.size() != 1)
		{
			digits_.resize(1);
		}

		digits_[0] = 0;
	}

	bool natural::is_zero() const & noexcept
	{
		return digits_.size() == 1 && digits_.front() == 0;
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
		cell_type count;

		for (const auto &digit: digits_ | std::views::reverse)
		{
			count = 0;

			remainder <<= 1;
			remainder += digit;

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

		for (const auto &digit: digits_ | std::views::reverse)
		{
			ss << digit;
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

		if (digits_[position] >= numeral_system_base - digit)
		{
			digit -= numeral_system_base - digits_[position];
			digits_[position] = digit;
			add_digit(1, position + 1);
		}
		else
		{
			digits_[position] += digit;
		}
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