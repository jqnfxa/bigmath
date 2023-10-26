#pragma once

#include "natural/natural.hpp"

namespace big
{
	class integer {
		bool is_negative_;
		natural absolute_value_;
	public:
		integer(std::int64_t num = 0);

		explicit integer(bool is_negative, const natural &natural);

		explicit integer(bool is_negative, natural &&natural);

		bool operator==(const integer &other) const & noexcept;

		std::strong_ordering operator<=>(const integer &other) const & noexcept;

		integer &operator++() & noexcept;

		integer operator++(int) const & noexcept;

		integer &operator--() & noexcept;

		integer operator--(int) const & noexcept;

		integer &operator+=(const integer &other) & noexcept;

		integer &operator-=(const integer &other) &;

		integer &operator*=(const integer &other) & noexcept;

		integer &operator/=(const integer &other) &;

		// TODO how to take % from negative integer??
		//integer &operator%=(const integer &other) &;

		integer &operator<<=(std::size_t shift) &;

		integer &operator>>=(std::size_t shift) & noexcept;

		integer operator+(const integer &other) const & noexcept;

		integer operator-(const integer &other) const &;

		integer operator*(const integer &other) const & noexcept;

		integer operator/(const integer &other) const &;

		integer operator%(const integer &other) const &;

		integer operator<<(std::size_t shift) const &;

		integer operator>>(std::size_t shift) const & noexcept;

		[[nodiscard]] bool is_zero() const & noexcept;

		std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const integer &num);
	};
}