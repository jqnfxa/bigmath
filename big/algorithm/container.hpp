#pragma once

#include <vector>
#include <ranges>
#include <algorithm>


namespace big::algorithm
{
/**
 * Erases leading elements from back while given predicate is true.
 *
 * @tparam T         Vector value type
 * @tparam UnaryPred Predicate type
 *
 * @param xs   Container
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
}
