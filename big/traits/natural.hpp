#pragma once

#include "natural/natural.hpp"


namespace big::traits
{
template <typename T>
concept natural_like = std::same_as<std::remove_cvref_t<T>, natural>;
}
