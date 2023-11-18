#pragma once

#include "../polynomial/polynomial.hpp"
#include "../rational/rational.hpp"
#include "expression.hpp"


namespace big::parse
{
namespace
{
constexpr auto polynomial_parameter_token = 'x';
constexpr auto add_token = '+';
constexpr auto sub_token = '-';
}

struct polynomial_node
{
	std::size_t degree;
	big::rational coefficient;
};

void throw_if_bad_polynomial_node(std::string_view str)
{
	if (str.empty())
	{
		throw std::invalid_argument("empty node");
	}

	const auto npos = std::string_view::npos;
	const auto mul = str.find(detail::tokens.at(token_id::mul));
	const auto x = str.find(polynomial_parameter_token);
	const auto power_sign = x == npos ? npos : str.find(detail::tokens.at(token_id::pow), x + 1);
	const auto power = power_sign == npos ? npos : power_sign + 1;

	if (mul != npos && x < mul)
	{
		throw std::invalid_argument("unexpected '*' after 'x'");
	}

	if (x == npos && std::none_of(str.begin(), str.end(), detail::is_digit))
	{
		throw std::invalid_argument("invalid node");
	}

	if (mul != npos)
	{
		// *x^n
		if (mul == 0)
		{
			throw std::invalid_argument("failed to parse node: leading *");
		}
		// a*^n
		if (x == npos)
		{
			throw std::invalid_argument("failed to parse node: bad sequence");
		}
	}

	// axn or a*xn or ax^ or a*x^
	if ((power_sign == npos) ^ (power == npos))
	{
		throw std::invalid_argument("failed to parse node: bad sequence");
	}
}

[[nodiscard]] polynomial_node parse_polynomial_node(std::string_view str)
{
	throw_if_bad_polynomial_node(str);

	const auto npos = std::string_view::npos;
	const auto mul = str.find(detail::tokens.at(token_id::mul));
	const auto x = str.find(polynomial_parameter_token);
	const auto power_sign = x != npos ? str.find(detail::tokens.at(token_id::pow), x + 1) : npos;
	const auto power = power_sign != npos ? power_sign + 1 : npos;

	polynomial_node node{};

        auto coefficient = str;
	coefficient.remove_suffix(mul == npos ? (x == npos ? 0 : str.size() - x) : str.size() - mul);
	bool should_inverse_sign = coefficient.front() == sub_token;

	if (should_inverse_sign)
	{
		coefficient.remove_prefix(1);
	}

	if (coefficient.empty())
	{
		coefficient = "1";
	}

	node.coefficient = expression(coefficient).evaluate<rational>();

	if (should_inverse_sign)
	{
		node.coefficient.flip_sign();
	}

	auto degree = str;
	if (x == npos)
	{
		degree = "0";
	}
	else if (power_sign == npos)
	{
		degree = "1";
	}
	else
	{
		degree.remove_prefix(power);
	}

	node.degree = static_cast<std::size_t>(expression(degree).evaluate<natural>());

        return node;
}

void add_node(std::map<std::size_t, big::rational> &coefficients, std::string_view node)
{
	if (node.empty())
	{
		return;
	}

	try
	{
		const auto new_node = parse_polynomial_node(node);
		coefficients[new_node.degree] += new_node.coefficient;
	}
	catch (const std::invalid_argument &) {}
}

std::vector<std::string> split(std::string s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;

	for (std::size_t pos = 0; (pos = s.find(delimiter)) != std::string::npos; )
	{
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + 1);
	}

	tokens.push_back(s);
	return tokens;
}

void insert_plus_before_minus(std::string &s)
{
	for (std::size_t pos = 0; (pos = s.find(sub_token, pos)) != std::string::npos; )
	{
		s.insert(pos, 1, add_token);

		// skip inserted '+' and '-'
		pos += 2;
	}
}

[[nodiscard]] big::polynomial parse_polynomial_without_brackets(std::string expression)
{
	insert_plus_before_minus(expression);
	expression.erase(std::remove_if(expression.begin(), expression.end(), detail::is_blank), expression.end());

	std::map<std::size_t, big::rational> coefficients;

	for (const auto &node : split(expression, add_token))
	{
		add_node(coefficients, node);
	}

	return polynomial(coefficients);
}
}
