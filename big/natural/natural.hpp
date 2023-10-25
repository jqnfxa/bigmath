#pragma once

#include <cstdint>
#include <vector>
#include <ostream>

namespace big
{
	class natural {
		// TODO where it should be?
		static constexpr std::int32_t base = 10;

		using cell_type = std::uint8_t;
		using num_representation = std::vector<cell_type>;

		num_representation num_;

		void erase_leading_zeroes();

	public:
		natural();

		explicit natural(std::size_t num);

		std::strong_ordering operator<=>(const natural &other) const;

		natural &operator++();

		// TODO use different dummy?
		natural operator++(int);

		natural &operator--();

		// TODO use different dummy?
		natural operator--(int);

		natural &operator+=(const natural &other);

		natural &operator-=(const natural &other);

		natural &operator*=(const natural &other);

		natural &operator/=(const natural &other);

		natural &operator%=(const natural &other);

		natural &operator<<=(std::size_t shift);

		natural &operator>>=(std::size_t shift);

		natural operator+(const natural &other) const;

		natural operator-(const natural &other) const;

		natural operator*(const natural &other) const;

		natural operator/(const natural &other) const;

		natural operator%(const natural &other) const;

		natural operator<<(std::size_t shift) const;

		natural operator>>(std::size_t shift) const;

		[[nodiscard]] bool is_zero() const;

		[[nodiscard]] std::pair<natural, natural> divide_by(const natural &divisor) const;

		friend std::ostream &operator<<(std::ostream &out, const natural &num);
	};
};