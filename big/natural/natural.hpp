#pragma once

#include <cstdint>
#include <vector>
#include <compare>
#include <ostream>

namespace big
{
	template <typename T>
	concept unsigned_type = std::unsigned_integral<T>;

	class natural {
	public:
		using cell_type = std::uint32_t;
		using size_type = std::size_t;
		using num_representation = std::vector<cell_type>;
		static constexpr cell_type number_system_base = 1'000'000'000;
		static constexpr std::uint8_t bits_per_num = 9;

	private:
		num_representation digits_;

		void erase_leading_zeroes() & noexcept;

		void nullify() & noexcept;

		void add_digit(num_representation &num, cell_type digit, size_type position);

		// If num < digit then undefined behavior
		void subtract_digit(num_representation &num, cell_type digit, size_type position);

	public:
		template <unsigned_type type>
		natural(type num = 0) noexcept
		{
			do
			{
				digits_.push_back(num % number_system_base);
				num /= number_system_base;
			} while (num != 0);

			erase_leading_zeroes();
		}

		natural(const std::string &num);

		natural(const num_representation &num);

		bool operator==(const natural &other) const & noexcept;

		std::strong_ordering operator<=>(const natural &other) const & noexcept;

		[[nodiscard]] bool is_even() const & noexcept;

		natural &operator++() & noexcept;

		natural operator++(int) const & noexcept;

		natural &operator--() &;

		natural operator--(int) const &;

		natural &operator+=(const natural &other) & noexcept;

		natural &operator-=(const natural &other) &;

		natural &operator*=(const natural &other) & noexcept;

		natural &operator/=(const natural &other) &;

		natural &operator%=(const natural &other) &;

		natural &operator<<=(std::size_t shift) &;

		natural &operator>>=(std::size_t shift) & noexcept;

		natural operator+(const natural &other) const & noexcept;

		natural operator-(const natural &other) const &;

		natural operator*(const natural &other) const & noexcept;

		natural operator/(const natural &other) const &;

		natural operator%(const natural &other) const &;

		natural operator<<(std::size_t shift) const &;

		natural operator>>(std::size_t shift) const & noexcept;

		[[nodiscard]] bool is_zero() const & noexcept;

		[[nodiscard]] std::pair<natural, natural> long_div(const natural &divisor) const &;

		[[nodiscard]] std::string to_str() const & noexcept;

		friend std::ostream &operator<<(std::ostream &out, const natural &num) noexcept;
	};
}