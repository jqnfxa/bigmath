#pragma once

#include "../rational/rational.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/polynomial.hpp"

namespace big
{
class polynomial {
	std::vector<rational> coefficients_;

	// TODO: can be noexcept?
	constexpr void erase_leading_zeroes() &
	{
		namespace ranges = std::ranges;

		if (numeric::is_zero(*this))
		{
			return;
		}

		const auto &last = ranges::find_if_not(coefficients_ | std::views::reverse, [](const auto &coefficient) { return numeric::is_zero(coefficient); });
		if (last == ranges::rend(coefficients_))
		{
			coefficients_.resize(1);
			coefficients_.back() = 0;
			return;
		}

		const auto &distance = ranges::distance(ranges::rbegin(coefficients_), last);
		coefficients_.erase(ranges::next(ranges::begin(coefficients_), ranges::size(coefficients_) - distance), ranges::end(coefficients_));
	}

public:
	using size_type = std::size_t;

	[[nodiscard]] constexpr polynomial() noexcept
		: coefficients_(1, rational{})
	{}

	[[nodiscard]] constexpr explicit polynomial(const std::vector<rational> &coefficients)
	{
		if (std::ranges::empty(coefficients))
		{
			throw std::invalid_argument("coefficients cannot be empty");
		}

		coefficients_.assign(std::ranges::rbegin(coefficients), std::ranges::rend(coefficients));
		erase_leading_zeroes();
	}

	[[nodiscard]] constexpr bool is_zero() const & noexcept
	{
                return degree() == 0 && numeric::is_zero(major_coefficient());
        }

	[[nodiscard]] constexpr const rational &major_coefficient() const & noexcept
	{
                return coefficients_.back();
        }

	[[nodiscard]] constexpr const std::vector<rational> &coefficients() const & noexcept
	{
		return coefficients_;
	}

	[[nodiscard]] constexpr size_type degree() const & noexcept
	{
		return std::ranges::size(coefficients_) - 1;
	}

	[[nodiscard]] constexpr std::strong_ordering operator<=>(const polynomial &other) const & noexcept		
	{
		return degree() <=> other.degree();
	}

	[[nodiscard]] constexpr bool operator==(const polynomial &other) const & noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	[[nodiscard]] constexpr polynomial derivative() const & noexcept
	{
		polynomial temp(*this);
		numeric::polynomial::coefficient_at(temp, 0) *= 0;

		for (size_type i = 1; i < std::ranges::size(temp.coefficients_); ++i)
		{
			numeric::polynomial::coefficient_at(temp, i - 1) = numeric::polynomial::coefficient_at(temp, i) * i;
		}

		if (numeric::polynomial::degree(temp) > 0)
		{
			temp.coefficients_.pop_back();
		}

		return temp;
	}

	[[nodiscard]] polynomial normalize() const &;
	void normalize() &;
	[[nodiscard]] polynomial multiple_roots_to_simple() const &;

	[[nodiscard]] constexpr rational &at(size_type degree) &
	{
		return const_cast<rational &>(const_cast<const polynomial *>(this)->at(degree));
	}

	[[nodiscard]] constexpr const rational &at(size_type degree) const &
	{
		if (degree >= std::ranges::size(coefficients_))
		{
			throw std::out_of_range("degree is out of range");
		}

		return coefficients_[degree];
	}

	[[nodiscard]] constexpr rational &operator[](size_type degree) &
	{
		return at(degree);
	}

	[[nodiscard]] constexpr const rational &operator[](size_type degree) const &
	{
		return at(degree);
	}

	polynomial &operator+=(const polynomial &other) & noexcept;
	polynomial &operator-=(const polynomial &other) & noexcept;
	polynomial &operator*=(const polynomial &other) & noexcept;
	polynomial &operator/=(const polynomial &other) &;
	polynomial &operator%=(const polynomial &other) &;
	polynomial &operator<<=(size_type shift) &;

	[[nodiscard]] polynomial operator+(const polynomial &other) const noexcept;
	[[nodiscard]] polynomial operator-(const polynomial &other) const noexcept;
	[[nodiscard]] polynomial operator*(const polynomial &other) const noexcept;
	[[nodiscard]] polynomial operator/(const polynomial &other) const;
	[[nodiscard]] polynomial operator%(const polynomial &other) const;
	[[nodiscard]] polynomial operator<<(size_type shift) const;

	[[nodiscard]] std::string str() const;
	friend std::ostream &operator<<(std::ostream &os, const polynomial &polynomial);

	[[nodiscard]] std::pair<polynomial, polynomial> long_div(const polynomial &divisor) const &;

	template <traits::rational_like T>
	polynomial &operator*=(const T &scalar) & noexcept
	{
		for (auto &coefficient : coefficients_)
		{
			coefficient *= scalar;
		}

		erase_leading_zeroes();
		return *this;
	}

	template <traits::rational_like T>
	polynomial &operator/=(const T &scalar) &
	{
		for (auto &coefficient : coefficients_)
		{
			coefficient /= scalar;
		}

		erase_leading_zeroes();
		return *this;
	}

	template <traits::rational_like T>
	polynomial operator*(const T &scalar) noexcept
	{
		polynomial result(*this);
		result *= scalar;
		return result;
	}

	template <traits::rational_like T>
	polynomial operator/(const T &scalar)
	{
		polynomial result(*this);
		result /= scalar;
		return result;
	}
};
}
