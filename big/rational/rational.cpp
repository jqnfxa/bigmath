#include "rational.hpp"
#include "numeric/rational.hpp"
#include <sstream>

namespace big
{
rational &rational::operator<<=(std::size_t shift) &
{
	numerator_ <<= shift;
        return *this;
}
rational &rational::operator>>=(std::size_t shift) &
{
	numerator_ >>= shift;
	simplify_fraction();
        return *this;
}

rational rational::operator<<(std::size_t shift) const &
{
	rational result(*this);
	result <<= shift;
        return result;
}

rational rational::operator>>(std::size_t shift) const &
{
	rational result(*this);
	result >>= shift;
        return result;
}

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
