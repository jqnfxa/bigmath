#pragma once

#include <cstdint>
#include <vector>
#include <compare>
#include <ostream>

namespace big
{
	class natural {
		using cell_type = std::uint8_t;
		using num_representation = std::vector<cell_type>;

		num_representation num_;

		void erase_leading_zeroes() & noexcept;

		void nullify() & noexcept;

	public:
		static constexpr std::int32_t base = 10;

		natural();

		explicit natural(const std::string &num);

		explicit natural(std::size_t num);

		bool operator==(const natural &other) const & noexcept;

		std::strong_ordering operator<=>(const natural &other) const & noexcept;

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

		[[nodiscard]] std::pair<natural, natural> divide_by(const natural &divisor) const &;

		std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const natural &num);
	};
}