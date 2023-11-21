#pragma once

#include "../rational/rational.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/polynomial.hpp"
#include "../algorithm/container.hpp"
#include <map>


namespace big
{
/**
 * Big polynomial implementation.
 */
class polynomial : public conv::stringifiable<polynomial>
{
	std::vector<rational> coefficients_;

	constexpr void erase_leading_zeroes() &
	{
		return algorithm::erase_from_back_while(coefficients_, numeric::is_zero<typename decltype(coefficients_)::value_type>);
	}

	template <typename T>
	constexpr void throw_if_empty(const T &container) const
	{
		if (std::ranges::empty(container))
		{
			throw std::invalid_argument("coefficients cannot be empty");
		}
	}
public:
	using size_type = std::size_t;

	[[nodiscard]] constexpr polynomial() noexcept
		: coefficients_(1, rational{})
	{}

	[[nodiscard]] constexpr explicit polynomial(const std::vector<rational> &coefficients)
	{
		throw_if_empty(coefficients);
		coefficients_.assign(std::ranges::rbegin(coefficients), std::ranges::rend(coefficients));
		erase_leading_zeroes();
	}

	[[nodiscard]] explicit polynomial(const std::map<std::size_t, rational> &coefficients);

	/**
	 * Checks if the polynomial is zero.
	 *
	 * @return `true` if the polynomial is zero, `false` otherwise
	 */
	[[nodiscard]] constexpr bool is_zero() const & noexcept
	{
	        return degree() == 0 && numeric::is_zero(major_coefficient());
	}

	/**
	 * Retrieves the major coefficient of the polynomial
	 *
	 * @return Major coefficient of the polynomial
	 *
	 * @note LED_P_Q
	 */
	[[nodiscard]] constexpr const rational &major_coefficient() const & noexcept
	{
	        return coefficients_.back();
	}

	/**
	 * Gets the coefficients of the polynomial.
	 *
	 * @return Coefficients
	 */
	[[nodiscard]] constexpr const std::vector<rational> &coefficients() const & noexcept
	{
		return coefficients_;
	}

	/**
	 * Returns the degree of the polynomial.
	 *
	 * @return Polynomial's degree
	 *
	 * @note DEG_P_N
	 */
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

	/**
	 * Retrieve coefficient corresponding to the given degree.
	 *
	 * @param degree Given degree
	 *
	 * @return Coefficient at the specified degree
	 */
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

	/**
	 * Compute the derivative of the polynomial.
	 *
	 * @return Derivative of the polynomial
	 *
	 * @note DER_P_P
	 */
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

	/**
	 * Returns a normalized version of the polynomial.
	 *
	 * @return Normalized version of the polynomial
	 *
	 * @note FAC_P_Q
	 */
	constexpr polynomial normalized() const
	{
		polynomial tmp(*this);
		tmp.normalize();
		return tmp;
	}

	/**
	 * Normalizes a polynomial by dividing by a rational number
	 * whose numerator is the GCD of all numerators of the coefficients of the polynomial,
	 * and whose denominator is the LCM of all denominators of the coefficients of the polynomial
	 */
	constexpr void normalize() &
	{
		auto scalar = numeric::polynomial::coefficient_at(*this, 0);

		for (auto &coefficient : coefficients_)
		{
			scalar *= numeric::sign(coefficient);
			scalar.numerator() = algorithm::gcd(numeric::rational::numerator(scalar), numeric::rational::numerator(coefficient));
			scalar.denominator() = algorithm::lcm(numeric::rational::denominator(scalar), numeric::rational::denominator(coefficient));
		}

		*this /= scalar;
	}

	/**
	 * Convert polynomials with multiple roots to one with simple roots.
	 *
	 * This function performs the following operations sequentially:
	 * 1. Creates a temporary copy of the given polynomial
	 * 2. Divides the copied polynomial by the GCD of itself and its derivative
	 * 3. Normalizes the copied polynomial i.e., divides the polynomial by
	 *    a rational number whose numerator is the GCD of all numerators of
	 *    the coefficients of the polynomial and whose denominator is the LCM
	 *    of all denominators of the coefficients of the polynomial
	 *
	 * @return The resultant polynomial after performing the above operations
	 *
	 * @note NMR_P_P
	 */
	constexpr polynomial multiple_roots_to_simple() const
	{
		polynomial tmp(*this);
		tmp /= algorithm::gcd(tmp, derivative());
		tmp.normalize();
		return tmp;
	}

	/**
	 * @note ADD_PP_P
	 */
	constexpr polynomial &operator+=(const polynomial &other) & noexcept
	{
		const auto other_degree = other.degree();
		coefficients_.resize(std::ranges::max(degree(), other_degree) + 1);

		for (size_type i = 0; i < other_degree + 1; ++i)
		{
			numeric::polynomial::coefficient_at(*this, i) += numeric::polynomial::coefficient_at(other, i);
		}

		erase_leading_zeroes();
		return *this;
	}

	/**
	 * @note SUB_PP_P
	 */
	constexpr polynomial &operator-=(const polynomial &other) & noexcept
	{
		*this *= -1;
		*this += other;
		*this *= -1;

		return *this;
	}

	/**
	 * @note MUL_PP_P
	 */
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

	/**
	 * @note DIV_PP_P
	 */
	constexpr polynomial &operator/=(const polynomial &other) &
	{
		*this = long_div(other).first;
		return *this;
	}

	/**
	 * @note MOD_PP_P
	 */
	constexpr polynomial &operator%=(const polynomial &other) &
	{
		*this = long_div(other).second;
		return *this;
	}

	/**
	 * Performs multiplication of a polynomial by x^k.
	 * The method shifts a polynomial up by adding k zeros at the beginning of the polynomial,
	 * which is equivalent to multiplying it by x^k.
	 *
	 * @param shift The number of positions 'k' by which the polynomial is to be shifted higher.
	 *
	 * @return Reference to instance
	 *
	 * @note MUL_Pxk_P
	 */
	constexpr polynomial &operator<<=(size_type shift) &
	{
		big::algorithm::shift_coefficients(coefficients_, shift);
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

	/**
	 * Performs polynomial long division and returns the quotient and the remainder.
	 *
	 * @param divisor The divisor polynomial for the division
	 *
	 * @return A pair of polynomials where the first element is the quotient and the second element is the remainder
	 */
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

	/**
	 * @note MUL_PQ_P
	 */
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
