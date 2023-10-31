#include "../big/polynomial/polynomial.hpp"
#include "algorithm/algorithm.hpp"
#include "gtest/gtest.h"

TEST(PolynomialTestSuite, PolynomialCreate)
{
	using namespace big;

	{
		std::vector<rational> test{rational{1, 1}, rational{1, 1}, rational{-5, 1}, rational{4, 1}};

		polynomial poly(std::vector<rational>{rational{1, 1}, rational{1, 1}, rational{-5, 1}, rational{4, 1}});

		auto cmp = poly.coefficients();
		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.begin(), cmp.end(), exp.begin(), exp.end()));

		ASSERT_EQ(poly.major_coefficient(), rational(1, 1));

		poly *= rational(15, 7);

		ASSERT_EQ(poly.major_coefficient(), rational(15, 7));
	}
	{
		std::vector<rational> test{rational{0, 1}};

		polynomial poly;

		auto cmp = poly.coefficients();
		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.begin(), cmp.end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialGcd)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1, 1), rational(2, 1), rational(3, 1), rational(6, 1)};
		std::vector<rational> p2 = {rational(1, 1), rational(2, 1)};
		std::vector<rational> test = {rational(1, 1), rational(2, 1)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(0, 1), rational(1, 1), rational(0, 1), rational(-3, 1),
									rational(-3, 1), rational(8, 1), rational(2, 1), rational(-5, 1)};
		std::vector<rational> p2 = {rational(3, 1), rational(0, 1), rational(5, 1), rational(0, 1), rational(-4, 1),
									rational(-9, 1), rational(21, 1)};
		std::vector<rational> test = {rational(-1288744821, 543589225)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(5, 1), rational(6, 1), rational(9, 1), rational(2, 1)};
		std::vector<rational> p2 = {rational(1, 1), rational(3, 1), rational(1, 1), rational(-2, 1), rational(-5, 1)};
		std::vector<rational> test = {rational(-335915, 87616)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(1, 1), rational(3, 1), rational(3, 1)};
		std::vector<rational> p2 = {rational(1, 1), rational(1, 1)};
		std::vector<rational> test = {rational(1, 1), rational(1, 1)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(2, 1), rational(3, 1), rational(6, 1)};
		std::vector<rational> p2 = {rational(1, 1), rational(2, 1)};
		std::vector<rational> test = {rational(1, 1), rational(2, 1)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialDerivative)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1, 1), rational(0, 1), rational(1, 1), rational(0, 1), rational(-3, 1),
									rational(-3, 1), rational(8, 1), rational(2, 1), rational(-5, 1)};
		std::vector<rational> p2 = {rational(8, 1), rational(0, 1), rational(6, 1), rational(0, 1), rational(-12, 1),
									rational(-9, 1), rational(16, 1), rational(2, 1)};

		polynomial poly(p1);
		polynomial der = poly.derivative();

		auto exp = p2;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(der.coefficients().begin(), der.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(15, 1)};
		std::vector<rational> p2 = {rational(0, 1)};

		polynomial poly(p1);
		polynomial der = poly.derivative();

		auto exp = p2;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(der.coefficients().begin(), der.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialPlus)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(3, 1), rational(4, 7), rational(-5, 1)};
		std::vector<rational> p2 = {rational(-5, 6), rational(5, 1)};

		polynomial cmp = polynomial(p1) + polynomial(p2);
		std::vector<rational> exp = {rational(3, 1), rational(-11, 42), rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(3, 1), rational(4, 7), rational(-5, 1)};
		std::vector<rational> p2 = {rational(-5, 6), rational(5, 6), rational(-5, 6), rational(5, 1)};

		polynomial cmp = polynomial(p1) + polynomial(p2);
		std::vector<rational> exp = {rational(-5, 6), rational(23, 6), rational(-11, 42), rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMinus)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(3, 1), rational(4, 7), rational(-5, 1)};
		std::vector<rational> p2 = {rational(-5, 6), rational(5, 1)};

		polynomial cmp = polynomial(p1) - polynomial(p2);
		std::vector<rational> exp = {rational(3, 1), rational(59, 42), rational(-10, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(3, 1), rational(4, 7), rational(-5, 1)};
		std::vector<rational> p2 = {rational(-5, 6), rational(5, 6), rational(-5, 6), rational(-5, 1)};

		polynomial cmp = polynomial(p1) - polynomial(p2);
		std::vector<rational> exp = {rational(5, 6), rational(13, 6), rational(59, 42), rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMulWithPolynomial)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1, 1), rational(6, 1), rational(-9, 1)};
		std::vector<rational> p2 = {rational(13, 8), rational(0, 1), rational(0, 1), rational(64, 1), rational(-4, 1)};

		polynomial cmp = polynomial(p1) * polynomial(p2);
		std::vector<rational> exp = {rational(13, 8), rational(39, 4), rational(-117, 8), rational(64, 1),
									 rational(380, 1), rational(-600, 1), rational(36, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(6, 1), rational(-9, 1)};
		std::vector<rational> p2 = {rational(0, 1)};

		polynomial cmp = polynomial(p1) * polynomial(p2);
		std::vector<rational> exp = {rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMulWithRational)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1, 1), rational(6, 1), rational(-9, 1)};

		polynomial cmp = polynomial(p1) * rational(7, 15);
		std::vector<rational> exp = {rational(7, 15), rational(14, 5), rational(-21, 5)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1, 1), rational(6, 1), rational(-9, 1)};

		polynomial cmp = polynomial(p1) * rational(0, 1);
		std::vector<rational> exp = {rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialShiftLeft)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(13, 8), rational(0, 1), rational(0, 1), rational(64, 1), rational(-4, 1)};

		polynomial cmp = polynomial(p1) << 0;
		std::vector<rational> exp = {rational(13, 8), rational(0, 1), rational(0, 1), rational(64, 1), rational(-4, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(13, 8), rational(0, 1), rational(0, 1), rational(64, 1), rational(-4, 1)};

		polynomial cmp = polynomial(p1) << 4;
		std::vector<rational> exp = {rational(13, 8), rational(0, 1), rational(0, 1), rational(64, 1), rational(-4, 1),
									 rational(0, 1), rational(0, 1), rational(0, 1), rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialDivide)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(138, 16), rational(34, 1), rational(0, 1), rational(0, 1), rational(0, 1),
									rational(19, 1), rational(14, 1), rational(0, 1)};
		std::vector<rational> p2 = {rational(16, 1), rational(0, 1), rational(0, 1), rational(1, 1)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(69, 128), rational(17, 8), rational(0, 1), rational(-69, 2048),
									 rational(-17, 128)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(14, 1), rational(0, 1), rational(-16, 1), rational(21, 1)};
		std::vector<rational> p2 = {rational(245, 1), rational(0, 1), rational(0, 1)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(2, 35), rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(-16, 1), rational(21, 1)};
		std::vector<rational> p2 = {rational(245, 1), rational(0, 1), rational(0, 1)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMod)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(138, 16), rational(34, 1), rational(0, 1), rational(0, 1), rational(0, 1),
									rational(19, 1), rational(14, 1), rational(0, 1)};
		std::vector<rational> p2 = {rational(16, 1), rational(0, 1), rational(0, 1), rational(1, 1)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(19, 1), rational(28741, 2048), rational(17, 128)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(14, 1), rational(0, 1), rational(-16, 1), rational(21, 1)};
		std::vector<rational> p2 = {rational(245, 1), rational(0, 1), rational(0, 1)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(-16, 1), rational(21, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(16, 1), rational(-236, 1), rational(692, 1), rational(1196, 1),
									rational(252, 1)};
		std::vector<rational> p2 = {rational(1, 1), rational(-16, 1), rational(63, 1)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(0, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialFac)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(9, 7), rational(6, 11), rational(3, 13)};
		polynomial cmp = polynomial(p1).fac();

		std::vector<rational> exp = {rational(429, 1), rational(182, 1), rational(77, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialNmr)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(6, 1), rational(-60, 1), rational(-120, 1), rational(2160, 1),
									rational(0, 1), rational(-20736, 1)};
		std::vector<rational> after_der = {rational(30, 1), rational(-240, 1), rational(-360, 1), rational(4320, 1),
										   rational(0, 1)};

		polynomial poly(p1);
		polynomial der = poly.derivative();

		std::reverse(after_der.begin(), after_der.end());
		ASSERT_TRUE(std::equal(der.coefficients().begin(), der.coefficients().end(), after_der.begin(), after_der.end()));

		polynomial cmp = poly.nmr();

		std::vector<rational> exp = {rational(1, 1), rational(-2, 1), rational(-24, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(7, 1), rational(0, 1), rational(-3, 1)};
		polynomial cmp = polynomial(p1).nmr();

		std::vector<rational> exp = {rational(7, 1), rational(0, 1), rational(-3, 1)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}