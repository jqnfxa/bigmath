#pragma once

#include <sstream>


namespace big::conv
{
template <typename T>
class stringifiable
{
public:
	[[nodiscard]] std::string str() const &
	{
		std::stringstream ss;
		ss << static_cast<const T &>(*this);
		return ss.str();
	}
};
}
