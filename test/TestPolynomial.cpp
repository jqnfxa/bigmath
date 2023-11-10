#include "../big/polynomial/polynomial.hpp"
#include "gtest/gtest.h"


TEST(PolynomialTestSuite, PolynomialCreate)
{
	using namespace big;

	{
		std::vector<rational> test{rational(1), rational(1), rational(-5), rational(4)};

		polynomial poly(std::vector<rational>{rational(1), rational(1), rational(-5), rational(4)});

		auto cmp = poly.coefficients();
		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.begin(), cmp.end(), exp.begin(), exp.end()));

		ASSERT_EQ(poly.major_coefficient(), rational(1));

		poly *= rational(15, 7u);

		ASSERT_EQ(poly.major_coefficient(), rational(15, 7u));
	}
	{
		std::vector<rational> test{rational(0)};

		polynomial poly;

		auto cmp = poly.coefficients();
		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.begin(), cmp.end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialDerivative)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1), rational(0), rational(1), rational(0), rational(-3),
									rational(-3), rational(8), rational(2), rational(-5)};
		std::vector<rational> p2 = {rational(8), rational(0), rational(6), rational(0), rational(-12),
									rational(-9), rational(16), rational(2)};

		polynomial poly(p1);
		polynomial der = poly.derivative();

		auto exp = p2;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(der.coefficients().begin(), der.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(15)};
		std::vector<rational> p2 = {rational(0)};

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
		std::vector<rational> p1 = {rational(3), rational(4, 7u), rational(-5)};
		std::vector<rational> p2 = {rational(-5, 6u), rational(5)};

		polynomial cmp = polynomial(p1) + polynomial(p2);
		std::vector<rational> exp = {rational(3), rational(-11, 42u), rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(3), rational(4, 7u), rational(-5)};
		std::vector<rational> p2 = {rational(-5, 6u), rational(5, 6u), rational(-5, 6u), rational(5)};

		polynomial cmp = polynomial(p1) + polynomial(p2);
		std::vector<rational> exp = {rational(-5, 6u), rational(23, 6u), rational(-11, 42u), rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMinus)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(3), rational(4, 7u), rational(-5)};
		std::vector<rational> p2 = {rational(-5, 6u), rational(5)};

		polynomial cmp = polynomial(p1) - polynomial(p2);
		std::vector<rational> exp = {rational(3), rational(59, 42u), rational(-10)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(3), rational(4, 7u), rational(-5)};
		std::vector<rational> p2 = {rational(-5, 6u), rational(5, 6u), rational(-5, 6u), rational(-5)};

		polynomial cmp = polynomial(p1) - polynomial(p2);
		std::vector<rational> exp = {rational(5, 6u), rational(13, 6u), rational(59, 42u), rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMulWithPolynomial)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1), rational(6), rational(-9)};
		std::vector<rational> p2 = {rational(13, 8u), rational(0), rational(0), rational(64), rational(-4)};

		polynomial cmp = polynomial(p1) * polynomial(p2);
		std::vector<rational> exp = {rational(13, 8u), rational(39, 4u), rational(-117, 8u), rational(64),
									 rational(380), rational(-600), rational(36)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(6), rational(-9)};
		std::vector<rational> p2 = {rational(0)};

		polynomial cmp = polynomial(p1) * polynomial(p2);
		std::vector<rational> exp = {rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMulWithRational)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1), rational(6), rational(-9)};

		polynomial cmp = polynomial(p1) * rational(7, 15u);
		std::vector<rational> exp = {rational(7, 15u), rational(14, 5u), rational(-21, 5u)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(6), rational(-9)};

		polynomial cmp = polynomial(p1) * rational(0);
		std::vector<rational> exp = {rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialShiftLeft)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(13, 8u), rational(0), rational(0), rational(64), rational(-4)};

		polynomial cmp = polynomial(p1) << 0;
		std::vector<rational> exp = {rational(13, 8u), rational(0), rational(0), rational(64), rational(-4)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(13, 8u), rational(0), rational(0), rational(64), rational(-4)};

		polynomial cmp = polynomial(p1) << 4;
		std::vector<rational> exp = {rational(13, 8u), rational(0), rational(0), rational(64), rational(-4),
									 rational(0), rational(0), rational(0), rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialDivide)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(138, 16u), rational(34), rational(0), rational(0), rational(0),
									rational(19), rational(14), rational(0)};
		std::vector<rational> p2 = {rational(16), rational(0), rational(0), rational(1)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(69, 128u), rational(17, 8u), rational(0), rational(-69, 2048u),
									 rational(-17, 128u)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(14), rational(0), rational(-16), rational(21)};
		std::vector<rational> p2 = {rational(245), rational(0), rational(0)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(2, 35u), rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(-16), rational(21)};
		std::vector<rational> p2 = {rational(245), rational(0), rational(0)};

		polynomial cmp = polynomial(p1) / polynomial(p2);
		std::vector<rational> exp = {rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialMod)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(138, 16u), rational(34), rational(0), rational(0), rational(0),
									rational(19), rational(14), rational(0)};
		std::vector<rational> p2 = {rational(16), rational(0), rational(0), rational(1)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(19), rational(28741, 2048u), rational(17, 128u)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(14), rational(0), rational(-16), rational(21)};
		std::vector<rational> p2 = {rational(245), rational(0), rational(0)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(-16), rational(21)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(16), rational(-236), rational(692), rational(1196),
									rational(252)};
		std::vector<rational> p2 = {rational(1), rational(-16), rational(63)};

		polynomial cmp = polynomial(p1) % polynomial(p2);
		std::vector<rational> exp = {rational(0)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialFac)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(9, 7u), rational(6, 11u), rational(3, 13u)};
		polynomial cmp = polynomial(p1).normalize();

		std::vector<rational> exp = {rational(429), rational(182), rational(77)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(PolynomialTestSuite, PolynomialNmr)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(6), rational(-60), rational(-120), rational(2160),
									rational(0), rational(-20736)};
		std::vector<rational> after_der = {rational(30), rational(-240), rational(-360), rational(4320),
										   rational(0)};

		polynomial poly(p1);
		polynomial der = poly.derivative();

		std::reverse(after_der.begin(), after_der.end());
		ASSERT_TRUE(std::equal(der.coefficients().begin(), der.coefficients().end(), after_der.begin(), after_der.end()));

		polynomial cmp = poly.multiple_roots_to_simple();

		std::vector<rational> exp = {rational(1), rational(-2), rational(-24)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(7), rational(0), rational(-3)};
		polynomial cmp = polynomial(p1).multiple_roots_to_simple();

		std::vector<rational> exp = {rational(7), rational(0), rational(-3)};
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}
