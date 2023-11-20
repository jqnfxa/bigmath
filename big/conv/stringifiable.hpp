#pragma once

#include <sstream>


namespace big::conv
{
template <typename T>
class stringifiable
{
public:
	/**
	 * Stringifies the instance.
	 *
	 * @return String representation of the class instance
	 *
	 * @note This invokes argument-dependent look up for the
	 *       overload of operator<< for `std::stringstream`.
	 */
	[[nodiscard]] std::string str() const &
	{
		std::stringstream ss;
		ss << static_cast<const T &>(*this);
		return ss.str();
	}
};
}
