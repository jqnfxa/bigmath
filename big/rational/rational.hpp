#pragma once

#include "../traits/traits.hpp"
#include "../numeric/numeric.hpp"
#include "../numeric/rational.hpp"
#include "../natural/natural.hpp"
#include "../integer/integer.hpp"
#include "../algorithm/algorithm.hpp"


namespace big
{
	class rational {
		integer numerator_;
		natural denominator_;

		constexpr void throw_if_denominator_is_zero()
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
		rational()  noexcept : rational(0)
		{}

		template <numeric::rational::rationalisable T>
		[[nodiscard]] constexpr rational(const T &other) noexcept
			: numerator_(std::move(numeric::rational::numerator(other)))
			, denominator_(std::move(numeric::rational::denominator(other)))
		{
			simplify_fraction();
		}

		template <traits::integer_like T, traits::integer_like U>
		[[nodiscard]] constexpr rational(const T &numerator = 0, const U &denominator = 1)
			: numerator_(numeric::abs(numerator), numeric::sign_bit(numerator) ^ numeric::sign_bit(denominator))
			, denominator_(numeric::abs(denominator))
		{
			throw_if_denominator_is_zero();
			simplify_fraction();
		}

		template <numeric::rational::rationalisable T>
		[[nodiscard]] constexpr std::strong_ordering operator<=>(const T &other) const noexcept
		{
			return numerator_ * numeric::rational::denominator(other) <=> numeric::rational::numerator(other) * denominator_;
		}

		template <numeric::rational::rationalisable T>
		[[nodiscard]] constexpr bool operator==(const T &other) const noexcept
		{
			return *this <=> other == std::strong_ordering::equal;
		}

		constexpr void flip_sign() & noexcept
		{
			return numerator_.flip_sign();
		}

		[[nodiscard]] constexpr bool sign_bit() const noexcept 
		{
			return numerator_.sign_bit();
		}

		[[nodiscard]] constexpr bool is_integer() const & noexcept
		{
			return denominator_ == 1u;
		}

		[[nodiscard]] constexpr const integer &numerator() const & noexcept
		{
			return numerator_;
		}

		[[nodiscard]] constexpr const natural &denominator() const & noexcept
		{
			return denominator_;
		}

		[[nodiscard]] constexpr integer &numerator() & noexcept
		{
			return const_cast<integer &>(const_cast<const rational *>(this)->numerator());
		}

		[[nodiscard]] constexpr natural &denominator() & noexcept
		{
			return const_cast<natural &>(const_cast<const rational *>(this)->denominator());
		}

		[[nodiscard]] constexpr rational inverse() const &
		{
			return rational(denominator_, numerator_);
		}

		template <numeric::rational::rationalisable T>
		rational &operator+=(const T &other) & noexcept
		{
			numerator_ = numerator_ * numeric::rational::denominator(other) + numeric::rational::numerator(other) * denominator_;
			denominator_ *= numeric::rational::denominator(other);
			simplify_fraction();

			return *this;
		}

		template <numeric::rational::rationalisable T>
		rational &operator-=(const T &other) & noexcept
		{
			flip_sign();
			*this += other;
			flip_sign();

			return *this;
		}

		template <numeric::rational::rationalisable T>
		rational &operator*=(const T &other) & noexcept
		{
			numerator_ *= numeric::rational::numerator(other);
			denominator_ *= numeric::rational::denominator(other);
			simplify_fraction();

			return *this;
		}

		template <numeric::rational::rationalisable T>
		rational &operator/=(const T &other) &
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

		template <numeric::rational::rationalisable T>
		rational operator+(const T &other) const & noexcept
		{
			rational temp(*this);
			temp += other;
			return temp;
		}

                template <numeric::rational::rationalisable T>
		rational operator-(const T &other) const & noexcept
		{
			rational temp(*this);
			temp -= other;
			return temp;
		}

		template <numeric::rational::rationalisable T>
		rational operator*(const T &other) const & noexcept
		{
			rational temp(*this);
			temp *= other;
			return temp;
		}

		template <numeric::rational::rationalisable T>
		rational operator/(const T &other) const &
		{
			rational temp(*this);
			temp /= other;
			return temp;
		}

		[[nodiscard]] std::string str() const;
		friend std::ostream &operator<<(std::ostream &out, const rational &num);
	};
}
