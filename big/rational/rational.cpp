#include "rational.hpp"
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
	if (numeric::sign_bit(num))
	{
		out << '-';
	}

	return out << num.numerator() << '/' << num.denominator();
}
}
