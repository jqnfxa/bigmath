#include <ranges>
#include <sstream>
#include "integer.hpp"
#include "numeric/numeric.hpp"


namespace big
{
std::string integer::str() const
{
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream &operator<<(std::ostream &out, const integer &num)
{
	if (numeric::sign_bit(num))
	{
		out << '-';
	}

	return out << numeric::abs(num);
}
}
