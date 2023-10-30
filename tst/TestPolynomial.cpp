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

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialMinus)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialMulWithPolunomial)
{
	using namespace big;
	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialMulWithRational)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialShiftLeft)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialDivide)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialMod)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(PolynomialTestSuite, PolynomialFac)
{
	using namespace big;

	// TODO implement
	FAIL();
}