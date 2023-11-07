#include <algorithm>
#include <ranges>
#include <stdexcept>
#include <sstream>
#include <compare>
#include <cmath>
#include <iomanip>
#include "natural.hpp"


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

std::string natural::str() const
{
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}

std::ostream &operator<<(std::ostream &out, const natural &num)
{
	const auto rbegin = std::ranges::rbegin(num.digits_);
	const auto rend = std::ranges::rend(num.digits_);

	for (auto it = rbegin; it != rend; ++it)
	{
		if (it != rbegin)
		{
			out << std::setw(natural::bits_per_num) << std::setfill('0');
		}

		out << *it;
	}

	return out;
}
}
