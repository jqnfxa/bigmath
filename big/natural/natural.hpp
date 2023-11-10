#pragma once

#include <cstdint>
#include <vector>
#include <compare>
#include <ostream>
#include <ranges>
#include <algorithm>
#include <limits>


namespace big
{
class natural
{
public:
	using digit_type = std::uint32_t;
	using digits_type = std::vector<digit_type>;
	using size_type = std::size_t;

	static constexpr digit_type number_system_base = 1'000'000'000;
	static constexpr std::uint8_t bits_per_num = 9;

private:
	digits_type digits_;

	constexpr void erase_leading_zeroes() &
	{
		namespace ranges = std::ranges;

		if (is_zero())
		{
			return;
		}

		auto last = ranges::find_if_not(digits_ | std::views::reverse, [](const auto &digit) { return digit == 0; });
		if (last == ranges::rend(digits_))
		{
			nullify();
			return;
		}

		const auto distance = ranges::distance(ranges::rbegin(digits_), last);
		digits_.erase(ranges::next(ranges::begin(digits_), ranges::size(digits_) - distance), ranges::end(digits_));
	}

	constexpr void nullify() &
	{
		if (digits_.size() != 1)
		{
			digits_.resize(1);
		}

		digits_.front() = 0;
	}

	constexpr void add_digit(digit_type digit, size_type position) &
	{
		const auto size = std::ranges::size(digits_);
		if (position > size)
		{
			throw std::out_of_range(std::to_string(position) + " is out of range");
		}

		for (; position < size; ++position)
		{
			const auto sum = digit + digits_[position];
			if (sum < number_system_base)
			{
				digits_[position] = sum;
				return;
			}

			digits_[position] = sum % number_system_base;
			digit = 1;
		}

		if (digit != 0)
		{
			digits_.back() = digit;
		}
	}

	constexpr void sub_digit(digit_type digit, size_type position) &
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

	// Since the method is private, it is not necessary to check division by 0.
	// This method is helper function for long_div and should used to find the quotient 
	// only when first > second
        constexpr natural find_quotient(const natural &first, const natural &second) const &
	{
		natural quotient{};

		if (std::ranges::size(second.digits_) == 1)
		{
			std::uintmax_t divisible = 0;

			for (const auto &digit : first.digits_ | std::views::reverse)
			{
				divisible *= number_system_base;
				divisible += digit;
			}

			quotient = divisible / second.digits_.front();
		}
		else
		{
			std::uintmax_t low = 1;
			std::uintmax_t high = number_system_base;

			while (low < high)
			{
				const auto mid = (low + high + 1) / 2;
				natural temp = second * mid;

				if (temp > first)
				{
					high = mid - 1;
				}
				else
				{
					low = mid;
				}
			}

			quotient = low;
		}

		return quotient;
	}
public:
	[[nodiscard]] constexpr natural(const digits_type &digits = {})
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

	template <std::unsigned_integral T = std::uintmax_t>
	[[nodiscard]] constexpr natural(T value) noexcept
	{
		do
		{
			digits_.push_back(value % number_system_base);
			value /= number_system_base;
		}
		while (value != 0);

		erase_leading_zeroes();
	}

	template <std::signed_integral T>
	[[nodiscard]] constexpr natural(T value) noexcept
		: natural(static_cast<std::uintmax_t>(std::abs(value)))
	{}

	[[nodiscard]] natural(const std::string &num);


	[[nodiscard]] constexpr bool is_even() const noexcept
	{
		return (digits_[0] & 1) == 0;
	}

	[[nodiscard]] constexpr bool is_zero() const noexcept
	{
		return digits_.size() == 1 && digits_.front() == 0;
	}

	[[nodiscard]] constexpr std::pair<natural, natural> long_div(const natural &divisor) const
	{
		if (divisor.is_zero())
		{
			throw std::domain_error("division by zero");
		}

		if (*this < divisor)
		{
			return {0, *this};
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

			// finding the maximum x such that: divisor * x <= remainder
			const auto x = find_quotient(remainder, divisor);

			quotient <<= 1;
			quotient += x;

			remainder -= divisor * x;
		}

		quotient.erase_leading_zeroes();
		remainder.erase_leading_zeroes();

		return {quotient, remainder};
	}

	[[nodiscard]] constexpr std::strong_ordering operator<=>(const natural &other) const noexcept
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

	[[nodiscard]] constexpr bool operator==(const natural &other) const noexcept = default;

	constexpr natural &operator++() &
	{
		return *this += 1;
	}

	constexpr natural operator++(int) const &
	{
		natural temp(*this);
		++temp;
		return temp;
	}

	constexpr natural &operator--() &
	{
		return *this -= 1;
	}

	constexpr natural operator--(int) const &
	{
		natural temp(*this);
		--temp;
		return temp;
	}

	constexpr natural &operator+=(const natural &other) &
	{
		if (this == &other)
		{
			return *this *= 2;
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

	constexpr natural &operator-=(const natural &other) &
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

	constexpr natural &operator*=(const natural &other) &
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

	constexpr natural &operator/=(const natural &other) &
	{
		digits_ = long_div(other).first.digits_;
		return *this;
	}

	constexpr natural &operator%=(const natural &other) &
	{
		digits_ = long_div(other).second.digits_;
		return *this;
	}

	constexpr natural &operator<<=(std::size_t shift) &
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

	constexpr natural &operator>>=(std::size_t shift) &
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

	[[nodiscard]] constexpr natural operator+(const natural &other) const
	{
		natural temp(*this);
		temp += other;
		return temp;
	}

	[[nodiscard]] constexpr natural operator-(const natural &other) const
	{
		natural temp(*this);
		temp -= other;
		return temp;
	}

	[[nodiscard]] constexpr natural operator*(const natural &other) const
	{
		natural temp(*this);
		temp *= other;
		return temp;
	}

	[[nodiscard]] constexpr natural operator/(const natural &other) const
	{
		natural temp(*this);
		temp /= other;
		return temp;
	}

	[[nodiscard]] constexpr natural operator%(const natural &other) const
	{
		natural temp(*this);
		temp %= other;
		return temp;
	}

	[[nodiscard]] constexpr natural operator<<(std::size_t shift) const
	{
		natural temp(*this);
		temp <<= shift;
		return temp;
	}

	[[nodiscard]] constexpr natural operator>>(std::size_t shift) const
	{
		natural temp(*this);
		temp >>= shift;
		return temp;
	}

	[[nodiscard]] std::string str() const;
	friend std::ostream &operator<<(std::ostream &out, const natural &num);
};
}
