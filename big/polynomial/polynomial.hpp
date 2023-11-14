#pragma once

#include "../rational/rational.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/polynomial.hpp"
#include "../algorithm/container.hpp"


namespace big
{
class polynomial : public conv::stringifiable<integer>
{
	std::vector<rational> coefficients_;

	constexpr void erase_leading_zeroes() &
	{
		return algorithm::erase_leading_up_to_last_if(coefficients_, numeric::is_zero<typename decltype(coefficients_)::value_type>);
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

	[[nodiscard]] constexpr polynomial derivative() const noexcept
	{
		polynomial der(*this);
		numeric::polynomial::coefficient_at(der, 0) *= 0;

		for (size_type i = 1; i < std::ranges::size(der.coefficients_); ++i)
		{
			numeric::polynomial::coefficient_at(der, i - 1) = numeric::polynomial::coefficient_at(der, i) * i;
		}

		if (numeric::polynomial::degree(der) > 0)
		{
			der.coefficients_.pop_back();
		}

		return der;
	}

	constexpr polynomial normalized() const
	{
		polynomial tmp(*this);
		tmp.normalize();
		return tmp;
	}

	constexpr void normalize() &
	{
		auto scalar = numeric::polynomial::coefficient_at(*this, 0);

		for (auto &coefficient : coefficients_)
		{
			scalar *= numeric::sign(coefficient);
			scalar.numerator() = gcd(numeric::rational::numerator(scalar), numeric::rational::numerator(coefficient));
			scalar.denominator() = lcm(numeric::rational::denominator(scalar), numeric::rational::denominator(coefficient));
		}

		*this /= scalar;
	}

	constexpr polynomial multiple_roots_to_simple() const &
	{
		polynomial tmp(*this);
		tmp /= gcd(tmp, derivative());
		tmp.normalize();
		return tmp;
	}

	constexpr polynomial &operator+=(const polynomial &other) & noexcept
	{
		coefficients_.resize(std::ranges::max(numeric::polynomial::degree(*this), numeric::polynomial::degree(other)) + 1);

		for (size_type i = 0; i < std::ranges::size(other.coefficients_); ++i)
		{
			numeric::polynomial::coefficient_at(*this, i) += numeric::polynomial::coefficient_at(other, i);
		}

		erase_leading_zeroes();
		return *this;
	}

	constexpr polynomial &operator-=(const polynomial &other) & noexcept
	{
		*this *= -1;
		*this += other;
		*this *= -1;

		return *this;
	}

	constexpr polynomial &operator*=(const polynomial &other) & noexcept
	{
		const auto &cur_len = std::ranges::size(coefficients_);
		const auto &other_len = std::ranges::size(other.coefficients_);

		std::vector<rational> result(cur_len + other_len - 1);

		for (size_type i = 0; i < cur_len; ++i)
		{
			for (size_type j = 0; j < other_len; ++j)
			{
				numeric::polynomial::coefficient_at(result, i + j) += numeric::polynomial::coefficient_at(*this, i) * numeric::polynomial::coefficient_at(other, j);
			}
		}

		coefficients_ = std::move(result);
		erase_leading_zeroes();

		return *this;
	}

	constexpr polynomial &operator/=(const polynomial &other) &
	{
		*this = long_div(other).first;
		return *this;
	}

	constexpr polynomial &operator%=(const polynomial &other) &
	{
		*this = long_div(other).second;
		return *this;
	}

	constexpr polynomial &operator<<=(size_type shift) &
	{
		const auto &size = std::ranges::size(coefficients_);

		if (size > coefficients_.max_size() - shift)
		{
			throw std::length_error("impossible to perform shift without losing data");
		}
		if (shift == 0)
		{
			return *this;
		}

		coefficients_.resize(size + shift);

		const auto rbegin = std::ranges::rbegin(coefficients_);
		std::copy(std::next(rbegin, shift), coefficients_.rend(), rbegin);
		std::fill_n(coefficients_.begin(), shift, rational{});

		return *this;
	}

	[[nodiscard]] constexpr polynomial operator+(const polynomial &other) const noexcept
	{
		polynomial tmp(*this);
		tmp += other;
		return tmp;
	}

	[[nodiscard]] constexpr polynomial operator-(const polynomial &other) const noexcept
	{
		polynomial tmp(*this);
		tmp -= other;
		return tmp;
	}

	[[nodiscard]] constexpr polynomial operator*(const polynomial &other) const noexcept
	{
		polynomial tmp(*this);
		tmp *= other;
		return tmp;
	}

	[[nodiscard]] constexpr polynomial operator/(const polynomial &other) const
	{
		polynomial tmp(*this);
		tmp /= other;
		return tmp;
	}

	[[nodiscard]] constexpr polynomial operator%(const polynomial &other) const
	{
		polynomial tmp(*this);
		tmp %= other;
		return tmp;
	}

	[[nodiscard]] constexpr polynomial operator<<(size_type shift) const
	{
		polynomial tmp(*this);
		tmp <<= shift;
		return tmp;
	}

	[[nodiscard]] constexpr std::pair<polynomial, polynomial> long_div(const polynomial &divisor) const &
	{
		if (numeric::is_zero(numeric::polynomial::degree(divisor)) && numeric::is_zero(divisor.major_coefficient()))
		{
			throw std::logic_error("Cannot divide by 0");
		}
		if (numeric::polynomial::degree(divisor) > numeric::polynomial::degree(*this))
		{
			return {polynomial{}, *this};
		}

		polynomial remainder = *this;
		polynomial quotient{};

		quotient <<= numeric::polynomial::degree(remainder) - numeric::polynomial::degree(divisor);

		while (numeric::polynomial::degree(remainder) >= numeric::polynomial::degree(divisor) && !numeric::is_zero(remainder.major_coefficient()))
		{
			const auto &new_coefficient = remainder.major_coefficient() / divisor.major_coefficient();
			const auto &degree = numeric::polynomial::degree(remainder) - numeric::polynomial::degree(divisor);

			numeric::polynomial::coefficient_at(quotient, degree) = new_coefficient;

			polynomial subtract({new_coefficient});
			subtract <<= degree;
			subtract *= divisor;

			remainder -= subtract;
		}

		quotient.erase_leading_zeroes();
		remainder.erase_leading_zeroes();

		return {quotient, remainder};
	}

	template <traits::rational_like T>
	constexpr polynomial &operator*=(const T &scalar) & noexcept
	{
		for (auto &coefficient : coefficients_)
		{
			coefficient *= scalar;
		}

		erase_leading_zeroes();
		return *this;
	}

	template <traits::rational_like T>
	constexpr polynomial &operator/=(const T &scalar) &
	{
		for (auto &coefficient : coefficients_)
		{
			coefficient /= scalar;
		}

		erase_leading_zeroes();
		return *this;
	}

	template <traits::rational_like T>
	constexpr polynomial operator*(const T &scalar) noexcept
	{
		polynomial result(*this);
		result *= scalar;
		return result;
	}

	template <traits::rational_like T>
	constexpr polynomial operator/(const T &scalar)
	{
		polynomial result(*this);
		result /= scalar;
		return result;
	}

	friend std::ostream &operator<<(std::ostream &os, const polynomial &polynomial);
};
}
