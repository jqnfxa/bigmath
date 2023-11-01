#include <ranges>
#include <stdexcept>
#include <sstream>
#include <compare>
#include <cmath>
#include <iomanip>
#include "natural.hpp"

namespace big
{
	natural::natural(std::uintmax_t num) noexcept
	{
		do
		{
			digits_.push_back(num % number_system_base);
			num /= number_system_base;
		} while (num != 0);

		erase_leading_zeroes();
	}

	natural::natural(const std::string &num)
	{
		if (num.empty())
		{
			throw std::invalid_argument("cannot build num from empty string");
		}

		const auto &is_not_a_decimal_digit = [](const auto &ch)
		{
			return !isdigit(ch);
		};

		if (std::any_of(num.begin(), num.end(), is_not_a_decimal_digit))
		{
			throw std::invalid_argument("invalid number, one or more characters are not a digit");
		}

		size_type offset = 0;
		const auto &chunk_size = bits_per_num;
		const auto &total_chunks = static_cast<size_type>(std::ceil(static_cast<double>(num.size()) / chunk_size));

		auto current_chunk_size = num.size() % chunk_size;
		if (current_chunk_size == 0)
		{
			current_chunk_size = chunk_size;
		}

		for (size_type i = 0; i < total_chunks; ++i)
		{
			auto chunk = num.substr(offset, current_chunk_size);

			cell_type digit = std::strtoul(chunk.data(), reinterpret_cast<char **>(chunk.data() + chunk.size()), 10);
			digits_.insert(digits_.begin(), digit);

			offset += current_chunk_size;
			current_chunk_size = chunk_size;
		}
	}

	natural::natural(const num_representation &num)
	{
		const auto &is_invalid_digit = [&](const auto &digit)
		{
			return digit >= number_system_base;
		};

		if (std::any_of(num.begin(), num.end(), is_invalid_digit))
		{
			throw std::invalid_argument(
				"an invalid number, one or more digits cannot be represented in the number system "
				+ std::to_string(number_system_base));
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
		// TODO handle situation a += a properly
		if (*this == other)
		{
			*this *= 2;
			return *this;
		}

		digits_.resize(std::max(digits_.size(), other.digits_.size()) + 1);

		for (size_type i = 0; i < other.digits_.size(); ++i)
		{
			add_digit(digits_, other.digits_[i], i);
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
		// TODO handle situation a -= a properly
		if (*this == other)
		{
			nullify();
			return *this;
		}

		for (size_type i = 0; i < other.digits_.size(); ++i)
		{
			subtract_digit(digits_, other.digits_[i], i);
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
				product += result[i + j];

				if (product >= number_system_base)
				{
					add_digit(result, product / number_system_base, i + j + 1);
				}

				result[i + j] = product % number_system_base;
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

		const auto &rbegin = digits_.rbegin();
		std::copy(std::next(rbegin, shift), digits_.rend(), rbegin);
		std::fill_n(digits_.begin(), shift, 0);

		return *this;
	}

	natural &natural::operator>>=(std::size_t shift) & noexcept
	{
		if (shift == 0)
		{
			return *this;
		}
		if (is_zero() || shift >= digits_.size())
		{
			nullify();
			return *this;
		}

		const auto &size = digits_.size();
		const auto &begin = digits_.begin();

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

	bool natural::is_zero() const & noexcept
	{
		return digits_.size() == 1 && digits_.front() == 0;
	}

	// TODO rework this function
	// Complexity: O(n*(n + log2(m) * m * number_system_base)) ~ O(n^2 + n * m * log2(m)), where n = len(this), m = len(divisor)
	std::pair<natural, natural> natural::long_div(const natural &divisor) const &
	{
		if (divisor.is_zero())
		{
			throw std::domain_error("cannot divide by zero");
		}
		if (*this < divisor)
		{
			return {0, *this};
		}

		natural quotient = 0;
		natural remainder = 0;

		for (const auto &digit: digits_ | std::views::reverse)
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
				std::uintmax_t mid = (low + high + 1) / 2;
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

	std::string natural::to_str() const & noexcept
	{
		std::ostringstream stream;
		stream << *this;
		return stream.str();
	}

	std::ostream &operator<<(std::ostream &out, const natural &num)
	{
		const auto digits = static_cast<std::int16_t>(std::ceil(std::log10(natural::number_system_base)));

		for (auto it = num.digits_.rbegin(); it != num.digits_.rend(); ++it)
		{
			if (it != num.digits_.rbegin())
			{
				out << std::setw(digits) << std::setfill('0');
			}

			out << *it;
		}

		return out;
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

	// TODO rewrite without goto and actual recursion
	void natural::add_digit(num_representation &num, cell_type digit, std::size_t position)
	{
		if (position > num.size())
		{
			throw std::out_of_range(std::to_string(position) + " is out of range");
		}

		recursion:
		if (position == num.size())
		{
			num.push_back(digit);
			return;
		}

		if (num[position] >= number_system_base - digit)
		{
			digit -= number_system_base - num[position];
			num[position] = digit;

			digit = 1;
			++position;

			goto recursion;
		}
		else
		{
			num[position] += digit;
		}
	}

	void natural::subtract_digit(num_representation &num, natural::cell_type digit, std::size_t position)
	{
		if (position >= num.size())
		{
			throw std::out_of_range(std::to_string(position) + " is out of range");
		}

		if (num[position] < digit)
		{
			auto borrow_position = position + 1;

			while (borrow_position < num.size() && num[borrow_position] == 0)
			{
				++borrow_position;
			}

			--num[borrow_position];

			for (size_type j = borrow_position - 1; j > position; --j)
			{
				num[j] += number_system_base - 1;
			}

			num[position] += number_system_base - digit;
		}
		else
		{
			num[position] -= digit;
		}
	}
}