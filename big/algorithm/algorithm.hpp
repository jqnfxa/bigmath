#pragma once

#include "../natural/natural.hpp"
#include "../polynomial/polynomial.hpp"

namespace big
{
polynomial gcd(const polynomial &a, const polynomial &b);
natural gcd(const natural &a, const natural &b);

natural lcm(const natural &a, const natural &b);

rational pow(rational num, integer base);
}
