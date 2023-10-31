#pragma once

#include "natural/natural.hpp"
#include "integer/integer.hpp"

namespace big
{
	class rational {
		integer numerator_;
		natural denominator_;

	public:
		rational();

		explicit rational(const integer &num) noexcept;

		explicit rational(integer &&num) noexcept;

		rational(integer numerator, natural denominator);

		void reduce() & noexcept;

		[[nodiscard]] bool is_integer() const & noexcept;

		[[nodiscard]] const integer &numerator() const & noexcept;

		[[nodiscard]] const natural &denominator() const & noexcept;

		[[nodiscard]] rational inverse() const &;

		rational &operator+=(const rational &other) & noexcept;

		rational &operator-=(const rational &other) & noexcept;

		rational &operator*=(const rational &other) & noexcept;

		rational &operator/=(const rational &other) &;

		rational operator+(const rational &other) const & noexcept;

		rational operator-(const rational &other) const & noexcept;

		rational operator*(const rational &other) const & noexcept;

		rational operator/(const rational &other) const &;

		bool operator==(const rational &other) const & noexcept;
	};
}