#include "big/parse/expression.hpp"
#include <iostream>

enum class domain_type : char
{
	natural    = 'n',
	integer    = 'z',
	rational   = 'q',
	polynomial = 'p',
};

int main()
{
	char domain;
	std::cout << "domain (N/Z/Q/P): ";
	std::cin >> domain;
	std::string expr_str;
	std::getline(std::cin >> std::ws, expr_str);

	big::parse::expression expr(expr_str);
	switch (static_cast<domain_type>(std::tolower(domain)))
	{
	case domain_type::natural:
		std::cout << expr.evaluate<big::natural>() << '\n';
		break;

	// case domain_type::integer:
	// 	std::cout << expr.evaluate<big::integer>() << '\n';
	// 	break;

	// case domain_type::rational:
	// 	std::cout << expr.evaluate<big::rational>() << '\n';
	// 	break;

	// case domain_type::polynomial:
	// 	std::cout << expr.evaluate<big::polynomial>() << '\n';
	// 	break;
	default:
		std::cout << "unknown domain\n";
		break;
	}
}
