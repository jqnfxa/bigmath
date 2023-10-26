//
// Created by shard on 10/25/23.
//

#include <ranges>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <compare>
#include "natural.hpp"

namespace big
{
	natural::natural() : natural(0)
	{
	}

	natural::natural(const std::string &num)
	{
		num_representation new_num;
		new_num.reserve(num.size());

		for (const auto &item: std::ranges::reverse_view(num))
		{
			if (isdigit(item))
			{
				new_num.push_back(item - '0');
			}
		}

		num_ = std::move(new_num);
		erase_leading_zeroes();
	}

	natural::natural(std::size_t num) : natural(std::to_string(num))
	{
	}

	// TODO how to handle == and != in <=> operator?
	bool natural::operator==(const natural &other) const & noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	std::strong_ordering natural::operator<=>(const natural &other) const & noexcept
	{
		if (num_.size() != other.num_.size())
		{
			return num_.size() <=> other.num_.size();
		}

		size_t order = num_.size() - 1;

		for (size_t i = 0; i <= order; ++i)
		{
			if (num_[order - i] != other.num_[order - i])
			{
				return num_[order - i] <=> other.num_[order - i];
			}
		}

		// If all digits are equal, the numbers are equal
		return std::strong_ordering::equal;
	}

	natural &natural::operator++() & noexcept
	{
		*this += natural("1");
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
		*this -= natural("1");
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
		auto max_order = std::max(num_.size(), other.num_.size());
		num_.resize(max_order + 1);

		for (size_t i = 0; i < other.num_.size(); ++i)
		{
			num_[i] += other.num_[i];
			num_[i + 1] += num_[i] / base;
			num_[i] %= base;
		}
		for (size_t i = other.num_.size(); i < max_order; ++i)
		{
			num_[i + 1] += num_[i] / base;
			num_[i] %= base;
		}

		erase_leading_zeroes();

		return *this;
	}

	natural &natural::operator-=(const natural &other) &
	{
		if (other > *this)
		{
			throw std::domain_error("Unable to subtract the greater natural");
		}

		for (size_t i = 0; i < other.num_.size(); ++i)
		{
			// TODO Put it in a separate method?
			if (num_[i] < other.num_[i])
			{
				size_t borrow_position = i + 1;

				while (borrow_position < num_.size() && num_[borrow_position] == 0)
				{
					++borrow_position;
				}

				--num_[borrow_position];

				for (size_t j = borrow_position - 1; j > i; --j)
				{
					num_[j] += base - 1;
				}
				num_[i] += base;
			}

			num_[i] -= other.num_[i];
		}

		erase_leading_zeroes();

		return *this;
	}

	natural &natural::operator*=(const natural &other) & noexcept
	{
		if (is_zero() || other.is_zero())
		{
			// TODO how to assign zero value normally?
			nullify();
			return *this;
		}

		num_representation result(num_.size() + other.num_.size(), 0);

		auto cur_num_len = num_.size();
		auto other_num_len = other.num_.size();

		for (size_t i = 0; i < cur_num_len; ++i)
		{
			for (size_t j = 0; j < other_num_len; ++j)
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
		num_ = std::move(divide_by(other).first.num_);
		return *this;
	}

	natural &natural::operator%=(const natural &other) &
	{
		num_ = std::move(divide_by(other).second.num_);
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
	std::pair<natural, natural> natural::divide_by(const natural &divisor) const &
	{
		if (divisor.is_zero())
		{
			throw std::domain_error("Cannot divide by zero");
		}
		if (*this < divisor)
		{
			return {natural(0), *this};
		}

		natural quotient(0);
		natural remainder(0);
		natural count(0);

		for (auto it: std::ranges::reverse_view(num_))
		{
			remainder <<= 1;
			remainder += natural(it);

			while (remainder >= divisor)
			{
				remainder -= divisor;
				++count;
			}

			quotient <<= 1;
			quotient += count;
			count.nullify();
		}

		quotient.erase_leading_zeroes();

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
}