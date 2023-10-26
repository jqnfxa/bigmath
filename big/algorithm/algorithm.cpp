#include "algorithm.hpp"

namespace big
{
	natural gcd(const natural &a, const natural &b)
	{
		if (a < b)
		{
			return gcd(b, a);
		}

		natural first = a;
		natural second = b;

		while (!second.is_zero())
		{
			natural temp = std::move(second);
			second = std::move(first % temp);
			first = std::move(temp);
		}

		return first;
	}

	natural lcm(const natural &a, const natural &b)
	{
		return a * b / gcd(a, b);
	}
}
