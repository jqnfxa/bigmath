#pragma once

#include "../traits/traits.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/rational.hpp"
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"
#include "../algorithm/algorithm.hpp"


namespace big
{
/**
 * Big natural number implementation.
 */
class rational : public conv::stringifiable<rational>
{
	integer numerator_;
	natural denominator_;

	constexpr void throw_if_denominator_is_zero() const &
	{
		if (denominator_.is_zero())
		{
			throw std::domain_error("it is impossible to represent a fraction with denominator 0");
		}
	}

	/**
	 * Simplifies a fraction by reducing by the NOD of the numerator and denominator
	 *
	 * @note RED_Q_Q
	 */
	constexpr void simplify_fraction() & noexcept
	{
		const auto coefficient = gcd(numeric::abs(numerator_), numeric::abs(denominator_));
		numerator_ /= coefficient;
		denominator_ /= coefficient;
	}

public:
	/**
	 * @note TRANS_Z_Q
	 */
	template <traits::integer_like T = std::intmax_t, traits::integer_like U = std::uintmax_t>
	[[nodiscard]] constexpr rational(const T &numerator = 0, const U &denominator = 1)
		: numerator_(numeric::abs(numerator), numeric::sign_bit(numerator) ^ numeric::sign_bit(denominator))
		, denominator_(numeric::abs(denominator))
	{
		throw_if_denominator_is_zero();
		simplify_fraction();
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr std::strong_ordering operator<=>(const T &other) const noexcept
	{
		const auto first_numerator = integer(numeric::rational::numerator(*this)) * numeric::rational::denominator(other);
		const auto second_numerator = integer(numeric::rational::numerator(other)) * numeric::rational::denominator(*this);
		return first_numerator <=> second_numerator;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr bool operator==(const T &other) const noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	/**
	 * Checks the number for being zero.
	 *
	 * @return `true` if the number is a canonical zero, `false` otherwise.
	 */
	[[nodiscard]] constexpr bool is_zero() const noexcept
	{
		return numeric::is_zero(numerator_);
	}

	/**
	 * Flips the sign bit
	 */
	constexpr void flip_sign() & noexcept
	{
		return numerator_.flip_sign();
	}

	/**
	 * Gets the sign bit.
	 *
	 * @return 'true' if the sign is set, 'false' otherwise
	 */
	[[nodiscard]] constexpr bool sign_bit() const noexcept
	{
		return numeric::sign_bit(numerator_);
	}

	/**
	 * Gets the numerator of the rational number.
	 *
	 * @return numerator as a constant integer reference.
	 */
	[[nodiscard]] constexpr const integer &numerator() const noexcept
	{
		return numerator_;
	}

	/**
	 * Gets the denominator of the rational number.
	 *
	 * @return denominator as a constant natural reference.
	 */
	[[nodiscard]] constexpr const natural &denominator() const noexcept
	{
		return denominator_;
	}

	/**
	 * Gets the numerator of the rational number.
	 *
	 * @return numerator as an integer reference.
	 */
	[[nodiscard]] constexpr integer &numerator() noexcept
	{
		return const_cast<integer &>(const_cast<const rational *>(this)->numerator());
	}

	/**
	 * Gets the denominator of the rational number.
	 *
	 * @return denominator as a natural reference.
	 */
	[[nodiscard]] constexpr natural &denominator() noexcept
	{
		return const_cast<natural &>(const_cast<const rational *>(this)->denominator());
	}

	/**
	 * Returns the inverse of the current rational object.
	 *
	 * @return A rational number, which is the inverted version of the current object.
	 */
	[[nodiscard]] constexpr rational inverse() const
	{
		return rational(denominator_, numerator_);
	}

	/**
	 * @note ADD_QQ_Q
	 */
	template <traits::rational_like T>
	constexpr rational &operator+=(const T &other) & noexcept
	{
		numerator_ = numeric::rational::numerator(*this) * numeric::rational::denominator(other) + numeric::rational::numerator(other) * numeric::rational::denominator(*this);
		denominator_ *= numeric::rational::denominator(other);
		simplify_fraction();

		return *this;
	}

	/**
	 * @note SUB_QQ_Q
	 */
	template <traits::rational_like T>
	constexpr rational &operator-=(const T &other) & noexcept
	{
		flip_sign();
		*this += other;
		flip_sign();

		return *this;
	}

	/**
	 * @note MUL_QQ_Q
	 */
	template <traits::rational_like T>
	constexpr rational &operator*=(const T &other) & noexcept
	{
		numerator_ *= numeric::rational::numerator(other);
		denominator_ *= numeric::rational::denominator(other);
		simplify_fraction();

		return *this;
	}

	/**
	 * @note DIV_QQ_Q
	 */
	template <traits::rational_like T>
	constexpr rational &operator/=(const T &other) &
	{
		numerator_ *= numeric::sign(other);
		numerator_ *= numeric::rational::denominator(other);
		denominator_ *= numeric::abs(numeric::rational::numerator(other));

		throw_if_denominator_is_zero();
		simplify_fraction();

		return *this;
	}

	constexpr rational &operator<<=(std::size_t shift) &
	{
		numerator_ <<= shift;
		return *this;
	}

	constexpr rational &operator>>=(std::size_t shift) &
	{
		numerator_ >>= shift;
		simplify_fraction();
		return *this;
	}

	template <traits::rational_like T>
	constexpr rational &operator%=(const T &other) &
	{
		*this *= 0;
		return *this;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr rational operator+(const T &other) const
	{
		rational tmp(*this);
		tmp += other;
		return tmp;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr rational operator-(const T &other) const
	{
		rational tmp(*this);
		tmp -= other;
		return tmp;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr rational operator*(const T &other) const
	{
		rational tmp(*this);
		tmp *= other;
		return tmp;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr rational operator/(const T &other) const
	{
		rational tmp(*this);
		tmp /= other;
		return tmp;
	}

	template <traits::rational_like T>
	[[nodiscard]] constexpr rational operator%(const T &other) const
	{
		rational tmp(*this);
		tmp %= other;
		return tmp;
	}

	[[nodiscard]] constexpr rational operator<<(std::size_t shift) const
	{
		rational result(*this);
		result <<= shift;
		return result;
	}

	[[nodiscard]] constexpr rational operator>>(std::size_t shift) const
	{
		rational result(*this);
		result >>= shift;
		return result;
	}

	friend std::ostream &operator<<(std::ostream &out, const rational &num);

	template <std::integral T>
	[[nodiscard]] explicit operator T() const & noexcept
	{
		return static_cast<T>(numerator_ / denominator_);
	}
};
}
