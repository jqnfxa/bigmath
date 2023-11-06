#include "algorithm.hpp"

namespace big
{
[[nodiscard]] polynomial gcd(const polynomial &a, const polynomial &b)
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

[[nodiscard]] rational pow(rational num, integer base)
{
	if (base < 0)
	{
		base.flip_sign();
		return rational(1) / pow(num, base);
	}
	if (num.numerator().abs() == 1u)
	{
		if (num.numerator() < 0 && base.abs().is_even())
		{
			// TODO: flip_sign
			num *= rational(-1);
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
