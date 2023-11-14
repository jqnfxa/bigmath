#pragma once

#include "../polynomial/polynomial.hpp"
#include "../rational/rational.hpp"
#include "parse.hpp"


namespace big::parse
{
[[nodiscard]] std::map<std::size_t, big::rational> parse_polynomial_helper(const std::string& poly)
{
	std::map<std::size_t, big::rational> coefficients;
	std::string token;

	std::string coefficient;
	std::size_t degree = 0;

	char sign = '+';
	bool isCoefficient = true;

	for (const auto &c : poly) 
	{
		if (c == '+' || c == '-') 
		{
			if (!token.empty()) 
			{
				if (isCoefficient) 
				{
					coefficient = token;
					degree = 0;
				} 
				else 
				{
					degree = std::stoull(token);
				}

				coefficients[degree] += big::parse::expression((sign == '+' ? "" : "0-") + coefficient).evaluate<big::rational>();
			}

			token.clear();
			sign = c;
			isCoefficient = true;
		} 
		else if (c == 'x') 
		{
			if (token.empty()) 
			{
				coefficient = "1";
			} 
			else 
			{
				coefficient = token;
			}

			token.clear();
			isCoefficient = false;
		} 
		else if (c == '^') 
		{
			token.clear();
		} 
		else 
		{
			token += c;
		}
	}

	if (!token.empty()) 
	{
		if (isCoefficient) 
		{
			coefficient = token;
			degree = 0;
		}
		else 
		{
			degree = std::stoull(token);
		}
		
		coefficients[degree] += big::parse::expression((sign == '+' ? "" : "0-") + coefficient).evaluate<big::rational>();
	}

	return coefficients;
}

[[nodiscard]] big::polynomial parse_polynomial(const std::string& expression)
{
	auto dictionary = parse_polynomial_helper(expression);
	std::vector<big::rational> coefficients(std::prev(dictionary.end())->first + 1);

	for (std::size_t i = 0; i < coefficients.size(); ++i)
	{
		if (dictionary.find(i) == dictionary.end())
		{
			coefficients[i] = 0;
		}
		else
		{
			coefficients[i] = dictionary[i];
		}
	}

	std::reverse(coefficients.begin(), coefficients.end());

	return big::polynomial(coefficients);
}
}
