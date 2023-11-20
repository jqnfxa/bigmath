#pragma once

#include <vector>
#include <ranges>
#include <algorithm>


namespace big::algorithm
{
/**
 * Erases leading elements from back while given predicate is true.
 *
 * @tparam T	Vector value type
 * @tparam UnaryPred Predicate type
 *
 * @param xs	Container
 * @param pred Unary predicate
 */
template <typename T, typename UnaryPred>
constexpr void erase_from_back_while(std::vector<T>& xs, const UnaryPred &pred) noexcept
{
	namespace ranges = std::ranges;

	const auto size = ranges::size(xs);

	if (size == 0)
	{
		return xs.push_back(T{});
	}

	if (size == 1 && pred(*ranges::begin(xs)))
	{
		return;
	}

	const auto last = ranges::find_if_not(xs | std::views::reverse, pred);
	if (last == ranges::rend(xs))
	{
		xs.resize(1);
		xs[0] = T{};
		return;
	}

	const auto distance = ranges::distance(ranges::rbegin(xs), last);
	xs.erase(ranges::next(ranges::begin(xs), ranges::size(xs) - distance), ranges::end(xs));
};

/**
 * Shifts elements in array forward.
 *
 * @tparam T Vector value type
 *
 * @param xs Container
 * @param n  Shift amount
 *
 * @throws `std::length_error` if impossible to perform the shift
 */
template <typename T>
constexpr void shift_coefficients(std::vector<T>& xs, std::size_t n)
{
	const auto size = std::ranges::size(xs);

	if (size > xs.max_size() - n)
	{
		throw std::length_error("impossible to perform shift without losing data");
	}

	if (n == 0)
	{
		return;
	}

	xs.resize(size + n);

	std::ranges::copy(xs | std::views::reverse | std::views::drop(n), std::ranges::rbegin(xs));
	std::ranges::fill_n(std::ranges::begin(xs), shift, T{});
};
}
