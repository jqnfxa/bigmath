#pragma once

#include <cstdint>
#include <vector>
#include <compare>
#include <ostream>

namespace big
{
class natural {
public:
	using digit_type = std::uint32_t;
	using digits_type = std::vector<digit_type>;
	using size_type = std::size_t;

	static constexpr digit_type number_system_base = 1'000'000'000;
	static constexpr std::uint8_t bits_per_num = 9;

private:
	digits_type digits_;

	void erase_leading_zeroes() &;
	void nullify() &;

	void add_digit(digit_type digit, size_type position);

	// Behavior is undefined if `num` is less than `digit`.
	void sub_digit(digit_type digit, size_type position);

public:
	template <std::unsigned_integral T = std::uintmax_t>
	[[nodiscard]] natural(T num = 0) noexcept
	{
		do
		{
			digits_.push_back(num % number_system_base);
			num /= number_system_base;
		}
		while (num != 0);
	}

	[[nodiscard]] natural(const std::string &num);
	[[nodiscard]] natural(const digits_type &num);


	[[nodiscard]] bool is_even() const noexcept;
	[[nodiscard]] bool is_zero() const noexcept;

	[[nodiscard]] std::pair<natural, natural> long_div(const natural &divisor) const;


	[[nodiscard]] std::strong_ordering operator<=>(const natural &other) const noexcept;
	[[nodiscard]] bool operator==(const natural &other) const noexcept = default;

	natural &operator++() &;
	natural operator++(int) const &;

	natural &operator--() &;
	natural operator--(int) const &;

	natural &operator+=(const natural &other) &;
	natural &operator-=(const natural &other) &;

	natural &operator*=(const natural &other) &;
	natural &operator/=(const natural &other) &;

	natural &operator%=(const natural &other) &;

	natural &operator<<=(std::size_t shift) &;
	natural &operator>>=(std::size_t shift) &;

	[[nodiscard]] natural operator+(const natural &other) const;
	[[nodiscard]] natural operator-(const natural &other) const;

	[[nodiscard]] natural operator*(const natural &other) const;
	[[nodiscard]] natural operator/(const natural &other) const;

	[[nodiscard]] natural operator%(const natural &other) const;

	[[nodiscard]] [[nodiscard]] natural operator<<(std::size_t shift) const;
	[[nodiscard]] natural operator>>(std::size_t shift) const;


	[[nodiscard]] std::string str() const;

	friend std::ostream &operator<<(std::ostream &out, const natural &num);
};
}
