#include <ranges>
#include <sstream>
#include "integer.hpp"


namespace big
{
std::string integer::str() const noexcept
{
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream &operator<<(std::ostream &out, const integer &num) noexcept
{
	if (num.sign_bit())
	{
		out << '-';
	}

	return out << num.abs();
}
}
