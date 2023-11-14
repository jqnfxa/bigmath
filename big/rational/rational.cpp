#include "rational.hpp"
#include "numeric/rational.hpp"
#include <sstream>

namespace big
{
std::ostream &operator<<(std::ostream &out, const rational &num)
{
	out << numeric::rational::numerator(num);

	if (numeric::rational::is_integer(num))
	{
		return out;
	}

	return out << '/' << numeric::rational::denominator(num);
}
}
