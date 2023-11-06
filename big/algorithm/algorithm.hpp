#pragma once

#include <utility>
#include "../natural/natural.hpp"
#include "../polynomial/polynomial.hpp"


namespace big
{
[[nodiscard]] polynomial gcd(const polynomial &a, const polynomial &b);

[[nodiscard]] constexpr natural gcd(const natural &a, const natural &b)
{
	if (a < b)
	{
		return gcd(b, a);
	}

	natural first(a);
	natural second(b);

	while (second != 0)
	{
		first = std::exchange(second, first % second);
	}

	return first;
}

[[nodiscard]] constexpr natural lcm(const natural &a, const natural &b)
{
	natural result = a * b;
	result /= gcd(a, b);
	return result;
}

[[nodiscard]] rational pow(rational num, integer base);
}
