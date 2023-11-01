#pragma once

#include "natural/natural.hpp"

namespace big
{
	class rational;

	class integer {
		bool sign_bit_;
		natural abs_;

		void normalize() & noexcept;

	public:
		integer(std::intmax_t num = 0) noexcept;

		explicit integer(const natural &natural, bool is_negative = false) noexcept;

		explicit integer(natural &&natural, bool is_negative = false) noexcept;

		explicit integer(const rational &other) noexcept;

		bool operator==(const integer &other) const & noexcept;

		std::strong_ordering operator<=>(const integer &other) const & noexcept;

		void flip_sing() & noexcept;

		[[nodiscard]] bool sign_bit() const & noexcept;

		[[nodiscard]] const natural &abs() const & noexcept;

		integer operator-() const & noexcept;

		integer operator+() const & noexcept;

		integer &operator++() & noexcept;

		integer operator++(int) const & noexcept;

		integer &operator--() & noexcept;

		integer operator--(int) const & noexcept;

		integer &operator+=(const integer &other) & noexcept;

		integer &operator+=(const natural &other) & noexcept;

		integer &operator-=(const integer &other) &;

		integer &operator-=(const natural &other) &;

		integer &operator*=(const integer &other) & noexcept;

		integer &operator*=(const natural &other) & noexcept;

		integer &operator/=(const integer &other) &;

		integer &operator/=(const natural &other) &;

		integer &operator%=(const integer &other) &;

		integer &operator<<=(std::size_t shift) &;

		integer &operator>>=(std::size_t shift) & noexcept;

		integer operator+(const integer &other) const & noexcept;

		integer operator+(const natural &other) const & noexcept;

		integer operator-(const integer &other) const &;

		integer operator-(const natural &other) const &;

		integer operator*(const integer &other) const & noexcept;

		integer operator*(const natural &other) const & noexcept;

		integer operator/(const integer &other) const &;

		integer operator/(const natural &other) const &;

		integer operator%(const integer &other) const &;

		integer operator<<(std::size_t shift) const &;

		integer operator>>(std::size_t shift) const & noexcept;

		[[nodiscard]] bool is_zero() const & noexcept;

		[[nodiscard]] std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const integer &num) noexcept;
	};
}