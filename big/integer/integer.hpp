#pragma once

#include "natural/natural.hpp"

namespace big
{
	class rational;
	class integer;

	template <typename T>
	concept signed_type = std::signed_integral<T>;

	template <typename T>
	concept integer_type = std::is_same_v<T, integer> || std::is_same_v<T, natural>;

	class integer {
		bool sign_bit_;
		natural abs_;

		void normalize() & noexcept;

	public:
		// TODO std::abs will cause overflow when num == type::min?
		template <signed_type type>
		integer(type num = 0) noexcept : sign_bit_(num < 0), abs_(static_cast<std::uintmax_t>(std::abs(num)))
		{
		}

		integer(const natural &natural, bool is_negative = false) noexcept;

		integer(natural &&natural, bool is_negative = false) noexcept;

		explicit integer(const rational &other) noexcept;

		bool operator==(const integer &other) const & noexcept;

		std::strong_ordering operator<=>(const integer &other) const & noexcept;

		void flip_sing() & noexcept;

		[[nodiscard]] bool sign_bit() const & noexcept;

		[[nodiscard]] const natural &abs() const & noexcept;

		integer operator-() const & noexcept;

		integer operator+() const & noexcept;

		integer &operator++() & noexcept;

		integer operator++(int) const & noexcept;

		integer &operator--() & noexcept;

		integer operator--(int) const & noexcept;

		integer &operator+=(const integer &other) & noexcept;

		integer &operator+=(const natural &other) & noexcept;

		integer &operator-=(const integer &other) &;

		integer &operator-=(const natural &other) &;

		integer &operator*=(const integer &other) & noexcept;

		integer &operator*=(const natural &other) & noexcept;

		integer &operator/=(const integer &other) &;

		integer &operator/=(const natural &other) &;

		integer &operator%=(const integer &other) &;

		integer &operator<<=(std::size_t shift) &;

		integer &operator>>=(std::size_t shift) & noexcept;

		template <integer_type type>
		integer operator+(const type &other) const & noexcept
		{
			integer temp(*this);
			temp += other;
			return temp;
		}

		template <integer_type type>
		integer operator-(const type &other) const &
		{
			integer temp(*this);
			temp -= other;
			return temp;
		}

		template <integer_type type>
		integer operator*(const type &other) const & noexcept
		{
			integer temp(*this);
			temp *= other;
			return temp;
		}

		template <integer_type type>
		integer operator/(const type &other) const &
		{
			integer temp(*this);
			temp /= other;
			return temp;
		}

		integer operator%(const integer &other) const &;

		integer operator<<(std::size_t shift) const &;

		integer operator>>(std::size_t shift) const & noexcept;

		[[nodiscard]] bool is_zero() const & noexcept;

		[[nodiscard]] std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const integer &num) noexcept;
	};
}