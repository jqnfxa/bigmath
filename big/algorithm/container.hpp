#pragma once

#include <vector>
#include <ranges>
#include <algorithm>


namespace big::algorithm
{
template <typename T, typename UnaryPred>
constexpr void erase_leading_up_to_last_if(std::vector<T>& xs, const UnaryPred &pred) noexcept
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
		return;
	}

	const auto distance = ranges::distance(ranges::rbegin(xs), last);
	xs.erase(ranges::next(ranges::begin(xs), ranges::size(xs) - distance), ranges::end(xs));
};
}
