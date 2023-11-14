#pragma once

#include <algorithm>
#include <map>
#include <stack>
#include <queue>

#include "../traits/traits.hpp"
#include "../algorithm/algorithm.hpp"


namespace big::parse
{
enum class token_id
{
	none,
	add,
	sub,
	mul,
	div,
	pow,
	mod,
	gcd,
	lcm,
	shl,
	shr,
	compound_begin,
	compound_end,
	integer_literal,
};

struct token_info
{
	token_id ident;
	std::string_view str;

	[[nodiscard]] constexpr bool operator==(const token_info &other) const & noexcept = default;
	[[nodiscard]] constexpr bool operator!=(const token_info &other) const & noexcept = default;
};

namespace detail
{
const std::map<token_id, std::string_view> tokens{
	{token_id::add, "+"},
	{token_id::sub, "-"},
	{token_id::mul, "*"},
	{token_id::div, "/"},
	{token_id::pow, "^"},
	{token_id::mod, "mod"},
	{token_id::gcd, "gcd"},
	{token_id::lcm, "lcm"},
	{token_id::shr, ">>"},
	{token_id::shr, ">>"},
	{token_id::compound_begin, "("},
	{token_id::compound_end, ")"},
};

enum class operator_precedence : unsigned char
{
	lowest,
	low,
	medium,
	high,
	highest,
};

[[nodiscard]] constexpr std::strong_ordering operator<=>(const operator_precedence &lhs, const operator_precedence &rhs) noexcept
{
	return static_cast<unsigned char>(lhs) <=> static_cast<unsigned char>(rhs);
}

[[nodiscard]] bool is_blank(char ch) noexcept
{
	return std::isblank(ch);
}

[[nodiscard]] bool is_digit(char ch) noexcept
{
	return std::isdigit(ch);
}

[[nodiscard]] bool is_binary_operator(token_id id) noexcept
{
	return id != token_id{} && id != token_id::compound_begin && id != token_id::compound_end && id != token_id::integer_literal;
}

[[nodiscard]] bool is_additive_operator(token_id id) noexcept
{
	return id == token_id::add || id == token_id::sub;
}

[[nodiscard]] bool is_multiplicative_operator(token_id id) noexcept
{
	return is_binary_operator(id) && !is_additive_operator(id);
}

[[nodiscard]] operator_precedence precedence(token_id id) noexcept
{
	switch (id)
	{
	case token_id::add:
		return operator_precedence::low;

	case token_id::sub:
		return operator_precedence::lowest;

	case token_id::mul:
		return operator_precedence::medium;

	case token_id::div:
		return operator_precedence::medium;

	case token_id::pow:
		return operator_precedence::high;

	default:
		return operator_precedence::highest;
	}
}

[[nodiscard]] token_id match_token(std::string_view str) noexcept
{
	if (std::all_of(str.begin(), str.end(), detail::is_digit))
	{
		return token_id::integer_literal;
	}

	for (const auto& [tok_ident, tok_str] : tokens)
	{
		if (str == tok_str)
		{
			return tok_ident;
		}
	}

	return token_id::none;
}
}

class expression
{
	std::string_view expression_;

protected:
	token_info next()
	{
		if (expression_.empty())
		{
			return {};
		}

		if (detail::is_blank(expression_.front()))
		{
			const auto first = expression_.begin();
			const auto last = expression_.end();

			const auto next_token = std::find_if_not(first, last, detail::is_blank);
			if (next_token == last)
			{
				return {};
			}

			expression_.remove_prefix(std::distance(first, next_token));
		}

		std::string_view substr = expression_;

		token_id id{};
		while (!substr.empty() && (id = detail::match_token(substr)) == token_id{})
		{
			substr.remove_suffix(1);
		}

		expression_.remove_prefix(substr.size());
		return {id, substr};
	}

public:
	[[nodiscard]] explicit expression(std::string_view expression)
		: expression_(expression)
	{}

	[[nodiscard]] std::queue<token_info> parse()
	{
		std::queue<token_info> output;
		std::stack<token_info> operators;

		for (token_info token; (token = next()) != token_info{}; )
		{
			switch (token.ident)
			{
			case token_id::integer_literal:
				output.push(token);
				break;

			case token_id::compound_end:
				while (!operators.empty())
				{
					if (const auto &op = operators.top(); op.ident == token_id::compound_begin)
					{
						operators.pop();
						break;
					}
					else
					{
						output.push(op);
						operators.pop();
					}
				}

				break;

			case token_id::compound_begin:
				operators.push(token);
				break;

			default:
				while (!operators.empty())
				{
					if (const auto &op = operators.top(); detail::is_binary_operator(op.ident) && detail::precedence(op.ident) >= detail::precedence(token.ident))
					{
						output.push(op);
						operators.pop();
					}
					else
					{
						break;
					}
				}

				operators.push(token);
				break;
			}
		}

		while (!operators.empty())
		{
			output.push(operators.top());
			operators.pop();
		}

		return output;
	}

	template <traits::rational_like T>
	[[nodiscard]] T evaluate()
	{
		auto tokens = parse();
		std::stack<T> values;

		const auto evaluate_operation = [&values](token_id ident)
		{
			const T rhs = values.top();
			values.pop();
			T lhs = values.top();
			values.pop();

			switch (ident)
			{
			case token_id::add:
				lhs += rhs;
				break;

			case token_id::sub:
				lhs -= rhs;
				break;

			case token_id::mul:
				lhs *= rhs;
				break;

			case token_id::div:
				lhs /= rhs;
				break;

			case token_id::pow:
				lhs = pow(lhs, rhs);
				break;

			case token_id::mod:
				lhs %= rhs;
				break;

			case token_id::gcd:
				lhs = gcd(lhs, rhs);
				break;

			case token_id::lcm:
				lhs = lcm(lhs, rhs);
				break;

			case token_id::shl:
				lhs <<= static_cast<std::size_t>(rhs);
				break;

			case token_id::shr:
				lhs >>= static_cast<std::size_t>(rhs);
				break;
			}

			return lhs;
		};

		while (!tokens.empty())
		{
			const auto &[ident, str] = tokens.front();

			values.push(ident == token_id::integer_literal ? T(str) : evaluate_operation(ident));

			tokens.pop();
		}

		if (values.empty())
		{
			throw std::invalid_argument("value stack is empty");
		}

		return values.top();
	}
};
}
