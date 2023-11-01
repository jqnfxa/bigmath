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

		if ((a % first).major_coefficient() == rational(0) && (b % first).major_coefficient() == rational(0))
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
		natural result = std::move(a * b);
		result /= gcd(a, b);
		return result;
	}

	rational pow(rational num, integer base)
	{
		if (base < 0)
		{
			base.flip_sing();
			return rational(1) / pow(num, base);
		}
		if (num.numerator().abs() == 1u)
		{
			if (num.numerator() < 0 && base.abs().is_even())
			{
				num *= -1;
			}

			return num;
		}

		rational result(1);

		while (base > 0)
		{
			if (base.abs().is_even())
			{
				num *= num;
				base /= 2;
			}
			else
			{
				result *= num;
				--base;
			}
		}

		return result;
	}
}
