#pragma once

#include "../traits/traits.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/rational.hpp"
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"
#include "../algorithm/algorithm.hpp"

namespace big
{
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

	constexpr void simplify_fraction() & noexcept
	{
		const auto coefficient = gcd(numeric::abs(numerator_), numeric::abs(denominator_));
		numerator_ /= coefficient;
		denominator_ /= coefficient;
	}
public:
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

	[[nodiscard]] constexpr bool is_zero() const noexcept
	{
		return numeric::is_zero(numerator_);
	}

	constexpr void flip_sign() & noexcept
	{
		return numerator_.flip_sign();
	}

	[[nodiscard]] constexpr bool sign_bit() const noexcept
	{
		return numeric::sign_bit(numerator_);
	}

	[[nodiscard]] constexpr const integer &numerator() const noexcept
	{
		return numerator_;
	}

	[[nodiscard]] constexpr const natural &denominator() const noexcept
	{
		return denominator_;
	}

	[[nodiscard]] constexpr integer &numerator() noexcept
	{
		return const_cast<integer &>(const_cast<const rational *>(this)->numerator());
	}

	[[nodiscard]] constexpr natural &denominator() noexcept
	{
		return const_cast<natural &>(const_cast<const rational *>(this)->denominator());
	}

	[[nodiscard]] constexpr rational inverse() const
	{
		return rational(denominator_, numerator_);
	}

	template <traits::rational_like T>
	constexpr rational &operator+=(const T &other) & noexcept
	{
		numerator_ = numeric::rational::numerator(*this) * numeric::rational::denominator(other) + numeric::rational::numerator(other) * numeric::rational::denominator(*this);
		denominator_ *= numeric::rational::denominator(other);
		simplify_fraction();

		return *this;
	}

	template <traits::rational_like T>
	constexpr rational &operator-=(const T &other) & noexcept
	{
		flip_sign();
		*this += other;
		flip_sign();

		return *this;
	}

	template <traits::rational_like T>
	constexpr rational &operator*=(const T &other) & noexcept
	{
		numerator_ *= numeric::rational::numerator(other);
		denominator_ *= numeric::rational::denominator(other);
		simplify_fraction();

		return *this;
	}

	template <traits::rational_like T>
	constexpr rational &operator/=(const T &other) &
	{
		if (numeric::sign_bit(other))
		{
			flip_sign();
		}

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
