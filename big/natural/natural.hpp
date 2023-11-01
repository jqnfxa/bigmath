#pragma once

#include <cstdint>
#include <vector>
#include <compare>
#include <ostream>

namespace big
{
	class natural {
	public:
		using cell_type = std::uintmax_t;
		using num_representation = std::vector<cell_type>;
		static constexpr cell_type numeral_system_base = std::numeric_limits<cell_type>::max();

	private:
		num_representation digits_;

		void erase_leading_zeroes() & noexcept;

		void nullify() & noexcept;

		void add_digit(cell_type digit, std::size_t position = 0);

		// If num is less than digit then UB
		void subtract_digit(cell_type digit, std::size_t position = 0);
	public:
		natural(std::uint64_t num = 0) noexcept;

		natural(const std::string &num) noexcept;

		natural(const num_representation &num);

		bool operator==(const natural &other) const & noexcept;

		std::strong_ordering operator<=>(const natural &other) const & noexcept;

		[[nodiscard]] bool is_even() const & noexcept;

		natural &operator++() & noexcept;

		natural operator++(int) const & noexcept;

		natural &operator--() & noexcept;

		natural operator--(int) const & noexcept;

		natural &operator+=(const natural &other) & noexcept;

		natural &operator-=(const natural &other) &;

		natural &operator*=(const natural &other) & noexcept;

		natural &operator/=(const natural &other) &;

		natural &operator%=(const natural &other) &;

		natural &operator<<=(std::size_t shift) &;

		natural &operator>>=(std::size_t shift) & noexcept;

		natural operator+(const natural &other) const & noexcept;

		natural operator-(const natural &other) const &;

		natural operator*(const natural &other) const & noexcept;

		natural operator/(const natural &other) const &;

		natural operator%(const natural &other) const &;

		natural operator<<(std::size_t shift) const &;

		natural operator>>(std::size_t shift) const & noexcept;

		[[nodiscard]] bool is_zero() const & noexcept;

		[[nodiscard]] std::pair<natural, natural> long_div(const natural &divisor) const &;

		[[nodiscard]] std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const natural &num);
	};
}