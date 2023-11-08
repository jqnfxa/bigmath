#pragma once

#include "../natural/natural.hpp"
#include "../traits/traits.hpp"


namespace big
{
class integer
{
	bool sign_bit_;
	natural abs_;

	constexpr void normalize() & noexcept
	{
		if (is_zero())
		{
			sign_bit_ = false;
		}
	}

	template <traits::integral T, typename BinaryPred>
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
	template <std::integral T>
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

	template <traits::integral T>
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

	template <traits::integral T>
	[[nodiscard]] constexpr bool operator==(const T &other) const noexcept
	{
		return *this <=> other == std::strong_ordering::equal;
	}

	[[nodiscard]] constexpr bool sign_bit() const noexcept
	{
		return sign_bit_;
	}

	constexpr void flip_sign() & noexcept
	{
		sign_bit_ = !sign_bit_;
	}

	[[nodiscard]] constexpr const natural &abs() const noexcept
	{
		return abs_;
	}

	[[nodiscard]] constexpr integer operator+() const &
	{
		return *this;
	}

	[[nodiscard]] constexpr integer operator-() const &
	{
		return {abs(), !sign_bit_};
	}

	constexpr integer &operator++() &
	{
		return *this += 1;
	}

	constexpr integer operator++(int) const &
	{
		integer temp(*this);
		++temp;
		return temp;
	}

	constexpr integer &operator--() &
	{
		return *this -= 1;
	}

	constexpr integer operator--(int) const &
	{
		integer temp(*this);
		--temp;
		return temp;
	}

	template <traits::integral T>
	constexpr integer &operator+=(const T &other) &
	{
		return add(other, std::ranges::equal_to{});
	}

	template <traits::integral T>
	constexpr integer &operator-=(const T &other) &
	{
		return add(other, std::ranges::not_equal_to{});
	}

	template <traits::integral T>
	constexpr integer &operator*=(const T &other) &
	{
		sign_bit_ ^= numeric::sign_bit(other);
		abs_ *= numeric::abs(other);
		normalize();

		return *this;
	}

	template <traits::integral T>
	constexpr integer &operator/=(const T &other) &
	{
		sign_bit_ ^= numeric::sign_bit(other);
		abs_ /= numeric::abs(other);
		normalize();

		return *this;
	}

	template <traits::integral T>
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

	template <traits::integral T>
	constexpr integer operator+(const T &other) const noexcept
	{
		integer temp(*this);
		temp += other;
		return temp;
	}

	template <traits::integral T>
	constexpr integer operator-(const T &other) const
	{
		integer temp(*this);
		temp -= other;
		return temp;
	}

	template <traits::integral T>
	constexpr integer operator*(const T &other) const noexcept
	{
		integer temp(*this);
		temp *= other;
		return temp;
	}

	template <traits::integral T>
	constexpr integer operator/(const T &other) const
	{
		integer temp(*this);
		temp /= other;
		return temp;
	}

	template <traits::integral T>
	constexpr integer operator%(const T &other) const
	{
		integer temp(*this);
		temp %= other;
		return temp;
	}

	constexpr integer operator<<(std::size_t shift) const
	{
		integer temp(*this);
		temp <<= shift;
		return temp;
	}

	constexpr integer operator>>(std::size_t shift) const
	{
		integer temp(*this);
		temp >>= shift;
		return temp;
	}

	[[nodiscard]] constexpr bool is_zero() const noexcept
	{
		return abs_.is_zero();
	}

	[[nodiscard]] std::string str() const noexcept;
	friend std::ostream &operator<<(std::ostream &out, const integer &num) noexcept;
};
}
