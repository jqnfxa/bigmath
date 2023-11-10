#include "rational.hpp"
#include "numeric/rational.hpp"
#include <sstream>

namespace big
{
std::string rational::str() const
{
	std::ostringstream stream;
	stream << *this;
	return stream.str();
}

std::ostream &operator<<(std::ostream &out, const rational &num)
{
	out << numeric::rational::numerator(num);

	if (num.is_integer())
	{
		return out;
	}

	return out << '/' << numeric::rational::denominator(num);
}
}
