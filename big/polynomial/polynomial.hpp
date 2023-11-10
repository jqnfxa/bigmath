#pragma once

#include "../rational/rational.hpp"
#include "../numeric/numeric.hpp"


namespace big
{
	class polynomial {
		std::vector<rational> coefficients_;

		void shrink_to_fit() & noexcept;

	public:
		using size_type = std::size_t;

		polynomial() noexcept;

		explicit polynomial(const std::vector<rational> &coefficients);

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
		
		[[nodiscard]] polynomial derivative() const & noexcept;
		[[nodiscard]] polynomial normalize() const & noexcept;
		void normalize() & noexcept;
		[[nodiscard]] polynomial multiple_roots_to_simple() const & noexcept;


		[[nodiscard]] rational &at(size_type degree) &;
		[[nodiscard]] const rational &at(size_type degree) const &;


		rational &operator[](size_type degree) &;
		const rational &operator[](size_type degree) const &;


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

		
		[[nodiscard]] std::string to_str() const;
		friend std::ostream &operator<<(std::ostream &os, const polynomial &polynomial);


		[[nodiscard]] std::pair<polynomial, polynomial> long_div(const polynomial &divisor) const &;


		template <numeric::rational::rationalisable T>
		polynomial &operator*=(const T &scalar) & noexcept
		{
			for (auto &coefficient : coefficients_)
			{
				coefficient *= scalar;
			}

			shrink_to_fit();
			return *this;
		}

		template <numeric::rational::rationalisable T>
		polynomial &operator/=(const T &scalar) &
		{
			for (auto &coefficient : coefficients_)
			{
				coefficient /= scalar;
			}

			shrink_to_fit();
			return *this;
		}

		template <numeric::rational::rationalisable T>
		polynomial operator*(const T &scalar) noexcept
		{
			polynomial result(*this);
                        result *= scalar;
                        return result;
		}

		template <numeric::rational::rationalisable T>
		polynomial operator/(const T &scalar)
		{
			polynomial result(*this);
                        result /= scalar;
                        return result;
		}
	};
}
