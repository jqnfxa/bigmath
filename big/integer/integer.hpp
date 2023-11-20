#pragma once

#include "../natural/natural.hpp"
#include "../traits/integer.hpp"

namespace big
{
/**
 * Big integer number implementation.
 */
class integer : public conv::stringifiable<integer>
{
	bool sign_bit_;
	natural abs_;

	/**
	 * If `abs_` is zero, sets `sign_bit_` to `false`.
	 */
	constexpr void normalize() & noexcept
	{
		if (is_zero())
		{
			sign_bit_ = false;
		}
	}

	/**
	 * If given predicate is true for sign bits of two numbers,
	 * adds the absolute value of the second one and preserves the sign bit.
	 * Otherwise, sets the absolute value to be equal to distance
	 * between the two absolute values and flips the sign bit.
	 *
	 * @tparam T          Value type
	 * @tparam BinaryPred Binary predicate type
	 *
	 * @param other          Other value
	 * @param sign_predicate Sign predicate
	 *
	 * @return Reference to instance
	 */
	template <traits::integer_like T, typename BinaryPred>
	constexpr integer &add(const T &other, const BinaryPred &sign_predicate) & noexcept
	{
		const auto &other_abs = numeric::abs(other);

		if (sign_predicate(sign_bit_, numeric::sign_bit(other)))
		{
			abs_ += other_abs;
		}
		else
		{
			sign_bit_ ^= abs_ < other_abs;
			abs_ = numeric::distance(abs_, other_abs);
		}

		normalize();
		return *this;
	}

public:
	template <traits::integer_like T = std::intmax_t>
	[[nodiscard]] constexpr integer(T num = 0) noexcept
		: sign_bit_(numeric::sign_bit(num))
		, abs_(numeric::abs(num))
	{}

	[[nodiscard]] constexpr integer(const natural &natural, bool negative = false) noexcept
		: sign_bit_(negative)
		, abs_(natural)
	{}

	[[nodiscard]] constexpr integer(natural &&natural, bool negative = false) noexcept
		: sign_bit_(negative)
		, abs_(std::move(natural))
	{}

	template <traits::integer_like T>
	[[nodiscard]] constexpr std::strong_ordering operator<=>(const T &other) const noexcept
	{
		const auto other_sign_bit = numeric::sign_bit(other);

		if (sign_bit_ ^ other_sign_bit)
		{
			return sign_bit_ && !other_sign_bit ? std::strong_ordering::less : std::strong_ordering::greater;
		}

		const auto other_abs = numeric::abs(other);
		return sign_bit_ && other_sign_bit ? other_abs <=> abs_ : abs_ <=> other_abs;
	}

	template <traits::integer_like T>
	[[nodiscard]] constexpr bool operator==(const T &other) const noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	/**
	 * Gets the sign bit.
	 *
	 * @return Sign bit
	 */
	[[nodiscard]] constexpr bool sign_bit() const noexcept
	{
		return sign_bit_;
	}

	/**
	 * Flips the sign bit.
	 */
	constexpr void flip_sign() & noexcept
	{
		sign_bit_ = !sign_bit_;
	}

	/**
	 * Gets the absolute value.
	 *
	 * @return Absolute value
	 */
	[[nodiscard]] constexpr const natural &abs() const noexcept
	{
		return abs_;
	}

	/**
	 * Checks the number for being zero.
	 *
	 * @return `true` if the number is a canonical zero, `false` otherwise.
	 */
	[[nodiscard]] constexpr bool is_zero() const noexcept
	{
		return abs_.is_zero();
	}

	[[nodiscard]] constexpr integer operator+() const noexcept
	{
		return *this;
	}

	[[nodiscard]] constexpr integer operator-() const noexcept
	{
		return {abs(), !sign_bit_};
	}

	constexpr integer &operator++() &
	{
		return *this += 1;
	}

	constexpr integer operator++(int) const &
	{
		integer tmp(*this);
		++tmp;
		return tmp;
	}

	constexpr integer &operator--() &
	{
		return *this -= 1;
	}

	constexpr integer operator--(int) const &
	{
		integer tmp(*this);
		--tmp;
		return tmp;
	}

	template <traits::integer_like T>
	constexpr integer &operator+=(const T &other) &
	{
		return add(other, std::ranges::equal_to{});
	}

	template <traits::integer_like T>
	constexpr integer &operator-=(const T &other) &
	{
		return add(other, std::ranges::not_equal_to{});
	}

	template <traits::integer_like T>
	constexpr integer &operator*=(const T &other) &
	{
		sign_bit_ ^= numeric::sign_bit(other);
		abs_ *= numeric::abs(other);
		normalize();

		return *this;
	}

	template <traits::integer_like T>
	constexpr integer &operator/=(const T &other) &
	{
		sign_bit_ ^= numeric::sign_bit(other);
		abs_ /= numeric::abs(other);
		normalize();

		return *this;
	}

	template <traits::integer_like T>
	constexpr integer &operator%=(const T &other) &
	{
		abs_ %= numeric::abs(other);
		normalize();

		return *this;
	}

	constexpr integer &operator<<=(std::size_t shift) &
	{
		abs_ <<= shift;
		normalize();

		return *this;
	}

	constexpr integer &operator>>=(std::size_t shift) &
	{
		abs_ >>= shift;
		normalize();

		return *this;
	}

	template <traits::integer_like T>
	constexpr integer operator+(const T &other) const
	{
		integer tmp(*this);
		tmp += other;
		return tmp;
	}

	template <traits::integer_like T>
	constexpr integer operator-(const T &other) const
	{
		integer tmp(*this);
		tmp -= other;
		return tmp;
	}

	template <traits::integer_like T>
	constexpr integer operator*(const T &other) const
	{
		integer tmp(*this);
		tmp *= other;
		return tmp;
	}

	template <traits::integer_like T>
	constexpr integer operator/(const T &other) const
	{
		integer tmp(*this);
		tmp /= other;
		return tmp;
	}

	template <traits::integer_like T>
	constexpr integer operator%(const T &other) const
	{
		integer tmp(*this);
		tmp %= other;
		return tmp;
	}

	constexpr integer operator<<(std::size_t shift) const
	{
		integer tmp(*this);
		tmp <<= shift;
		return tmp;
	}

	constexpr integer operator>>(std::size_t shift) const
	{
		integer tmp(*this);
		tmp >>= shift;
		return tmp;
	}

	friend std::ostream &operator<<(std::ostream &out, const integer &num);

	template <std::integral T>
	[[nodiscard]] explicit operator T() const & noexcept
	{
		return numeric::sign(*this) * static_cast<T>(abs_);
	}
};
}
