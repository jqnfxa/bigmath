#pragma once

#include "../natural/natural.hpp"
#include "../integer/integer.hpp"

namespace big
{
	template <typename T>
	concept multiplicable_with_rational = std::is_same_v<T, rational> ||
										  std::is_same_v<T, integer> ||
										  std::is_same_v<T, natural>;

	class rational {
		integer numerator_;
		natural denominator_;

	public:
		rational();

		explicit rational(integer numerator, natural denominator = 1u);

		void simplify_fraction() & noexcept;

		[[nodiscard]] bool is_integer() const & noexcept;

		// TODO numerator and denominator getter to be able to modify them (integer &, natural &) ?
		[[nodiscard]] const integer &numerator() const & noexcept;

		[[nodiscard]] const natural &denominator() const & noexcept;

		[[nodiscard]] rational inverse() const &;

		rational &operator+=(const rational &other) & noexcept;

		rational &operator-=(const rational &other) & noexcept;

		rational &operator*=(const rational &other) & noexcept;

		rational &operator*=(const integer &other) & noexcept;

		rational &operator*=(const natural &other) & noexcept;

		rational &operator/=(const rational &other) &;

		rational &operator/=(const integer &other) &;

		rational &operator/=(const natural &other) &;

		rational operator+(const rational &other) const & noexcept;

		rational operator-(const rational &other) const & noexcept;

		template <multiplicable_with_rational NumberType>
		rational operator*(const NumberType &other) const & noexcept
		{
			rational temp(*this);
			temp *= other;
			return temp;
		}

		template <multiplicable_with_rational NumberType>
		rational operator/(const NumberType &other) const &
		{
			rational temp(*this);
			temp /= other;
			return temp;
		}

		// TODO operator<=> instead
		bool operator==(const rational &other) const & noexcept;
	};
}
