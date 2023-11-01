#pragma once

#include "rational/rational.hpp"

namespace big
{
	class polynomial {
		std::vector<rational> coefficients_;

		void shrink_to_fit() & noexcept;

	public:
		using size_type = std::size_t;

		polynomial() noexcept;

		explicit polynomial(const std::vector<rational> &coefficients) noexcept;

		[[nodiscard]] const rational &major_coefficient() const & noexcept;

		[[nodiscard]] const std::vector<rational> &coefficients() const & noexcept;

		[[nodiscard]] std::size_t degree() const & noexcept;

		[[nodiscard]] polynomial derivative() const & noexcept;

		[[nodiscard]] polynomial normalize() const & noexcept;

		void normalize() & noexcept;

		[[nodiscard]] polynomial multiple_roots_to_simple() const & noexcept;

		rational &at(std::size_t degree) &;

		[[nodiscard]] const rational &at(std::size_t degree) const &;

		rational &operator[](std::size_t degree) &;

		const rational &operator[](std::size_t degree) const &;

		polynomial &operator+=(const polynomial &other) & noexcept;

		polynomial &operator-=(const polynomial &other) & noexcept;

		polynomial &operator*=(const rational &coefficient) & noexcept;

		polynomial &operator*=(const polynomial &other) & noexcept;

		polynomial &operator<<=(std::size_t shift) &;

		polynomial &operator/=(const polynomial &other) &;

		polynomial &operator/=(const rational &coefficient) &;

		polynomial &operator%=(const polynomial &other) &;

		polynomial operator+(const polynomial &other) const & noexcept;

		polynomial operator-(const polynomial &other) const & noexcept;

		polynomial operator*(const rational &coefficient) const & noexcept;

		polynomial operator*(const polynomial &other) const & noexcept;

		polynomial operator<<(std::size_t shift) const &;

		polynomial operator/(const polynomial &other) const &;

		polynomial operator/(const rational &coefficient) const &;

		polynomial operator%(const polynomial &other) const &;

		[[nodiscard]] std::pair<polynomial, polynomial> long_div(const polynomial &divisor) const &;
	};
}