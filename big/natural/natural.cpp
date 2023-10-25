//
// Created by shard on 10/25/23.
//

#include <ranges>
#include <stdexcept>
#include <limits>
#include "natural.hpp"

namespace big
{
	natural::natural() : natural(0)
	{
	}

	natural::natural(std::size_t num)
	{
		num_representation new_num;

		do
		{
			new_num.push_back(num % base);
			num /= base;
		} while (num != 0);

		num_ = std::move(new_num);
	}

	std::strong_ordering natural::operator<=>(const natural &other) const
	{
		if (num_.size() != other.num_.size())
		{
			return num_.size() <=> other.num_.size();
		}

		size_t order = num_.size() - 1;

		while (order != 0 && num_[order] == other.num_[order])
		{
			--order;
		}

		return num_[order] <=> other.num_[order];
	}

	natural &natural::operator++()
	{
		*this += natural(1);
		return *this;
	}

	natural natural::operator++(int)
	{
		natural temp(*this);
		++(temp);
		return temp;
	}

	natural &natural::operator--()
	{
		*this -= natural(1);
		return *this;
	}

	natural natural::operator--(int)
	{
		natural temp(*this);
		--(temp);
		return temp;
	}

	natural &natural::operator+=(const natural &other)
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

	natural &natural::operator-=(const natural &other)
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

	natural &natural::operator*=(const natural &other)
	{
		if (is_zero() || other.is_zero())
		{
			// TODO how to assign zero value normally?
			*this = natural(0);
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

	natural &natural::operator/=(const natural &other)
	{
		num_ = divide_by(other).first.num_;
		return *this;
	}

	natural &natural::operator%=(const natural &other)
	{
		num_ = divide_by(other).second.num_;
		return *this;
	}

	natural &natural::operator<<=(std::size_t shift)
	{
		if (num_.size() > std::numeric_limits<std::size_t>::max() - shift)
		{
			throw std::length_error("Impossible to perform shift without losing data");
		}

		if (is_zero() || shift == 0)
		{
			return *this;
		}

		auto new_size = num_.size() + shift;
		num_.resize(new_size);

		// TODO iterator::difference_type (long) vs shift (std::size_t)
		auto old = std::prev(num_.end(), shift);
		auto destination = std::prev(num_.end());

		for (; old != num_.begin(); --destination, --old)
		{
			*destination = *old;
		}
		for (; destination != num_.begin(); --destination)
		{
			*destination = 0;
		}

		return *this;
	}

	natural &natural::operator>>=(std::size_t shift)
	{
		if (is_zero() || shift == 0 || shift >= num_.size())
		{
			return *this;
		}

		// TODO iterator::difference_type (long) vs shift (std::size_t)
		auto old = num_.begin() + shift;

		for (auto destination = num_.begin(); old != num_.end(); ++destination, ++old)
		{
			*destination = *old;
		}

		auto new_size = num_.size() - shift;
		num_.resize(new_size);

		return *this;
	}

	natural natural::operator+(const natural &other) const
	{
		natural temp(*this);
		temp += other;
		return temp;
	}

	natural natural::operator-(const natural &other) const
	{
		natural temp(*this);
		temp -= other;
		return temp;
	}

	natural natural::operator*(const natural &other) const
	{
		natural temp(*this);
		temp *= other;
		return temp;
	}

	natural natural::operator/(const natural &other) const
	{
		natural temp(*this);
		temp /= other;
		return temp;
	}

	natural natural::operator%(const natural &other) const
	{
		natural temp(*this);
		temp %= other;
		return temp;
	}

	natural natural::operator<<(std::size_t shift) const
	{
		natural temp(*this);
		temp <<= shift;
		return temp;
	}

	natural natural::operator>>(std::size_t shift) const
	{
		natural temp(*this);
		temp >>= shift;
		return temp;
	}

	void natural::erase_leading_zeroes()
	{
		if (is_zero())
		{
			return;
		}

		auto first_non_zero = std::find_if(num_.rbegin(), num_.rend(), [](const auto &digit)
		{
			return digit != 0;
		});

		auto real_size = num_.size() - std::distance(num_.rbegin(), first_non_zero);
		num_.resize(real_size);
	}

	bool natural::is_zero() const
	{
		return num_.size() == 1 && num_.front() == 0;
	}

	// TODO function is terribly slow, improve required
	std::pair<natural, natural> natural::divide_by(const natural &divisor) const
	{
		if (divisor.is_zero())
		{
			throw std::domain_error("Cannot divide by zero");
		}

		natural quotient;
		natural remainder = *this;

		while (remainder >= divisor)
		{
			remainder -= divisor;
			++quotient;
		}

		return {quotient, remainder};
	}

	std::ostream &operator<<(std::ostream &out, const natural &num)
	{
		for (const auto &it: std::ranges::reverse_view(num.num_))
		{
			out << static_cast<int16_t>(it);
		}

		return out;
	}
}