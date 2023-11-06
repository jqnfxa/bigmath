#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <sstream>
#include <compare>
#include <cmath>
#include <iomanip>
#include "natural.hpp"
#include<iostream>
namespace big
{
natural::natural(const std::string &num)
{
	if (num.empty())
	{
		throw std::invalid_argument("cannot build num from empty string");
	}

	const auto &is_not_a_decimal_digit = [](const auto &ch)
	{
		return !std::isdigit(ch);
	};

	if (std::ranges::any_of(num, is_not_a_decimal_digit))
	{
		throw std::invalid_argument("invalid number, one or more characters are not a digit");
	}

	const auto &chunk_size = bits_per_num;
	auto current_chunk_size = std::ranges::size(num) % chunk_size;
	if (current_chunk_size == 0)
	{
		current_chunk_size = chunk_size;
	}

	size_type offset = 0;
	const size_type total_chunks = std::ceil(static_cast<double>(std::ranges::size(num)) / chunk_size);
	for (size_type i = 0; i < total_chunks; ++i)
	{
		auto chunk = num.substr(offset, current_chunk_size);

		digit_type digit = std::strtoul(chunk.data(), nullptr, 10);
		digits_.insert(std::ranges::begin(digits_), digit);

		offset += current_chunk_size;
		current_chunk_size = chunk_size;
	}

	erase_leading_zeroes();
}

natural::natural(const digits_type &digits)
{
	const auto &is_invalid_digit = [this](const auto &digit)
	{
		return digit >= number_system_base;
	};

	if (std::ranges::any_of(digits, is_invalid_digit))
	{
		throw std::invalid_argument(
			"an invalid number, one or more digits cannot be represented in the number system "
			+ std::to_string(number_system_base));
	}

	digits_ = digits;
	erase_leading_zeroes();
}

std::strong_ordering natural::operator<=>(const natural &other) const noexcept
{
	const auto this_size = std::ranges::size(digits_);
	const auto other_size = std::ranges::size(other.digits_);

	if (!this_size || !other_size || this_size != other_size)
	{
		return this_size <=> other_size;
	}

	const auto &order = this_size - 1;

	for (size_type i = 0; i <= order; ++i)
	{
		if (digits_[order - i] != other.digits_[order - i])
		{
			return digits_[order - i] <=> other.digits_[order - i];
		}
	}

	return std::strong_ordering::equal;
}

bool natural::is_even() const noexcept
{
	return ~(digits_[0] & 1);
}

natural &natural::operator++() &
{
	*this += 1u;
	return *this;
}

natural natural::operator++(int) const &
{
	natural temp(*this);
	++temp;
	return temp;
}

natural &natural::operator--() &
{
	*this -= 1u;
	return *this;
}

natural natural::operator--(int) const &
{
	natural temp(*this);
	--temp;
	return temp;
}

natural &natural::operator+=(const natural &other) &
{
	if (this == &other)
	{
		return *this *= 2u;
	}

	if (other.is_zero())
	{
		return *this;
	}

	const auto other_size = std::ranges::size(other.digits_);
	digits_.resize(std::max(std::ranges::size(digits_), other_size) + 1);

	for (size_type i = 0; i < other_size; ++i)
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

	if (this == &other)
	{
		nullify();
		return *this;
	}

	if (other.is_zero())
	{
		return *this;
	}

	for (size_type i = 0; i < std::ranges::size(other.digits_); ++i)
	{
		sub_digit(other.digits_[i], i);
	}

	erase_leading_zeroes();
	return *this;
}

natural &natural::operator*=(const natural &other) &
{
	if (is_zero() || other.is_zero())
	{
		nullify();
		return *this;
	}

	if (other == 1u)
	{
		return *this;
	}

	const auto this_size = std::ranges::size(digits_);
	const auto other_size = std::ranges::size(other.digits_);

	natural result{};
	result.digits_.resize(this_size + other_size, 0);

	for (size_type i = 0; i < this_size; ++i)
	{
		for (size_type j = 0; j < other_size; ++j)
		{
			std::uintmax_t product = digits_[i];
			product *= other.digits_[j];
			product += result.digits_[i + j];

			if (product >= number_system_base)
			{
				result.add_digit(product / number_system_base, i + j + 1);
			}

			result.digits_[i + j] = product % number_system_base;
		}
	}

	result.erase_leading_zeroes();
	digits_ = std::move(result.digits_);

	return *this;
}

natural &natural::operator/=(const natural &other) &
{
	digits_ = long_div(other).first.digits_;
	return *this;
}

natural &natural::operator%=(const natural &other) &
{
	digits_ = long_div(other).second.digits_;
	return *this;
}

natural &natural::operator<<=(std::size_t shift) &
{
	const auto &size = std::ranges::size(digits_);

	if (is_zero() || shift == 0)
	{
		return *this;
	}

	if (size + shift > std::numeric_limits<size_type>::max())
	{
		throw std::length_error("impossible to perform shift without losing data");
	}

	digits_.resize(size + shift);

	std::ranges::copy(digits_ | std::views::reverse | std::views::drop(shift), std::ranges::rbegin(digits_));
	std::ranges::fill_n(std::ranges::begin(digits_), shift, 0);

	return *this;
}

natural &natural::operator>>=(std::size_t shift) &
{
	if (shift == 0)
	{
		return *this;
	}

	if (shift >= digits_.size())
	{
		nullify();
		return *this;
	}

	const auto &size = std::ranges::size(digits_);
	const auto &begin = std::ranges::begin(digits_);

	digits_.erase(begin, std::ranges::next(begin, shift));
	digits_.resize(size - shift);

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

bool natural::is_zero() const noexcept
{
	return digits_.size() == 1 && digits_.front() == 0;
}

// TODO rework this function
// Complexity: O(n*(n + log2(m) * m * number_system_base)) ~ O(n^2 + n * m * log2(m)), where n = len(this), m = len(divisor)
std::pair<natural, natural> natural::long_div(const natural &divisor) const
{
	if (divisor.is_zero())
	{
		throw std::domain_error("division by zero");
	}

	if (*this < divisor)
	{
		return {0u, *this};
	}

	natural quotient{};
	natural remainder{};

	for (const auto &digit : digits_ | std::views::reverse)
	{
		remainder <<= 1;
		remainder += digit;

		if (remainder < divisor)
		{
			continue;
		}

		std::uintmax_t low = 1;
		std::uintmax_t high = number_system_base;

		while (low < high)
		{
			const std::uintmax_t mid = (low + high + 1) / 2;
			natural temp = divisor * mid;

			if (temp > remainder)
			{
				high = mid - 1;
			}
			else
			{
				low = mid;
			}
		}

		quotient <<= 1;
		quotient += low;

		remainder -= divisor * low;
	}

	quotient.erase_leading_zeroes();
	remainder.erase_leading_zeroes();

	return {quotient, remainder};
}

std::string natural::str() const
{
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}

std::ostream &operator<<(std::ostream &out, const natural &num)
{
	static auto length = std::ceil(std::log10(natural::number_system_base));

	const auto rbegin = std::ranges::rbegin(num.digits_);
	const auto rend = std::ranges::rend(num.digits_);

	for (auto it = rbegin; it != rend; ++it)
	{
		if (it != rbegin)
		{
			out << std::setw(length) << std::setfill('0');
		}

		out << *it;
	}

	return out;
}

void natural::erase_leading_zeroes() &
{
	if (is_zero())
	{
		return;
	}

	auto last = std::ranges::find_if_not(digits_ | std::views::reverse, [](const auto &digit) { return digit == 0; });
	if (last == std::ranges::rend(digits_))
	{
		nullify();
	}

	const auto distance = std::ranges::distance(std::ranges::rbegin(digits_), last);
	digits_.erase(std::ranges::next(std::ranges::begin(digits_), std::ranges::size(digits_) - distance), std::ranges::end(digits_));
}

void natural::nullify() &
{
	if (digits_.size() != 1)
	{
		digits_.resize(1);
	}

	digits_.front() = 0;
}

void natural::add_digit(digit_type digit, size_type position)
{
	const auto size = std::ranges::size(digits_);
	if (position > size)
	{
		throw std::out_of_range(std::to_string(position) + " is out of range");
	}

	for (; position <= size; ++position)
	{
		if (position == size)
		{
			return digits_.push_back(digit);
		}

		const auto sum = digit + digits_[position];
		if (sum < number_system_base)
		{
			digits_[position] = sum;
			return;
		}

		digits_[position] = sum % number_system_base;
		digit = 1;
	}
}

void natural::sub_digit(digit_type digit, size_type position)
{
	const auto size = std::ranges::size(digits_);
	if (position >= size)
	{
		throw std::out_of_range(std::to_string(position) + " is out of range");
	}

	if (digits_[position] >= digit)
	{
		digits_[position] -= digit;
		return;
	}

	auto borrow_position = position + 1;
	for (; borrow_position < size && digits_[borrow_position] == 0; ++borrow_position)
	{
		digits_[borrow_position] = number_system_base - 1;
	}

	--digits_[borrow_position];
	digits_[position] += number_system_base - digit;
}
}
