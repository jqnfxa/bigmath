#include <utility>
#include "algorithm.hpp"

namespace big
{
	polynomial gcd(const polynomial &a, const polynomial &b)
	{
		if (a.degree() < b.degree())
		{
			return gcd(b, a);
		}

		polynomial first = a;
		polynomial second = b;

		while (second.degree() != 0)
		{
			first = std::exchange(second, first % second);
		}

		if ((a % first).major_coefficient() == rational(0, 1) && (b % first).major_coefficient() == rational(0, 1))
		{
			return first;
		}

		return second;
	}

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
			first = std::exchange(second, first % second);
		}

		return first;
	}

	natural lcm(const natural &a, const natural &b)
	{
		return a * b / gcd(a, b);
	}
}
