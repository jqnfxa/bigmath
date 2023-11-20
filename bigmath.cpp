#include "big/parse/expression.hpp"
#include "big/parse/polynomial.hpp"
#include "big/algorithm/algorithm.hpp"
#include <iostream>
#include <functional>
#include <ranges>

enum class domain : char
{
	natural    = 'n',
	integer    = 'z',
	rational   = 'q',
	polynomial = 'p',
};

template <typename Proj = std::identity>
[[nodiscard]] auto read_projected_string(std::istream& is, const Proj &proj = std::identity{})
{
	std::string str;
	std::getline(is >> std::ws, str);

	return std::invoke(proj, str);
}

std::map<std::array<std::string_view, 2>, big::polynomial (big::polynomial::*)(const big::polynomial &) const> polynomial_operations{
	{{"+", "add"}, &big::polynomial::operator+},
	{{"-", "sub"}, &big::polynomial::operator-},
	{{"*", "mul"}, &big::polynomial::operator*},
	{{"/", "div"}, &big::polynomial::operator/},
	{{"%", "mod"}, &big::polynomial::operator%},
};

std::map<std::string_view, big::polynomial(*)(const big::polynomial &, const big::polynomial &)> polynomial_binary_algorithms{
	{"gcd", &big::algorithm::gcd},
	{"lcm", &big::algorithm::lcm},
};

int main()
{
	char in_domain;
	std::cout << "domain (N/Z/Q/P): ";
	std::cin >> in_domain;

	try
	{
		const auto domain_type = static_cast<domain>(std::tolower(in_domain));
		if (domain_type == domain::polynomial)
		{
			std::cout << "available operations over polynomials: \n"
				"der, gcd, lcm, + (add), - (sub), * (mul), / (div), % (mod)\n";

			std::cout << "p(x) = ";
			const big::polynomial p = read_projected_string(std::cin, big::parse::parse_polynomial_without_brackets);

			std::cout << "operation: ";
			const auto operation_str = read_projected_string(std::cin);

			if (operation_str == "der")
			{
				std::cout << p.derivative() << '\n';
				return 0;
			}

			std::cout << "q(x) = ";
			const big::polynomial q = read_projected_string(std::cin, big::parse::parse_polynomial_without_brackets);


			const auto binary_algorithm_proj = &std::ranges::range_value_t<decltype(polynomial_binary_algorithms)>::first;
			if (const auto b = std::ranges::find(polynomial_binary_algorithms, operation_str, binary_algorithm_proj); b != std::ranges::end(polynomial_binary_algorithms))
			{
				std::cout << b->second(p, q);
			}
			else
			{
				for (const auto &[keys, fn] : polynomial_operations)
				{
					if (std::ranges::find(keys, operation_str) != std::ranges::end(keys))
					{
						std::cout << (p.*fn)(q);
						break;
					}
				}
			}

			return 0;
		}

		auto expr = read_projected_string(std::cin, [](std::string s) { return big::parse::expression(s); });
		switch (domain_type)
		{
		case domain::natural:
			std::cout << expr.evaluate<big::natural>() << '\n';
			break;

		case domain::integer:
			std::cout << expr.evaluate<big::integer>() << '\n';
			break;

		case domain::rational:
			std::cout << expr.evaluate<big::rational>() << '\n';
			break;

		default:
			std::cout << "unknown domain\n";
			break;
		}
	}
	catch (const std::exception &e)
	{
		std::cout << "error: " << e.what() << '\n';
	}
}
