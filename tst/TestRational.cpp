#include "../big/rational/rational.hpp"
#include "gtest/gtest.h"

TEST (RationalTestSuite, TestCreation)
{
	using namespace big;

	try
	{
		rational a(1, -0);

		FAIL();
	}
	catch (const std::invalid_argument &e)
	{
		EXPECT_EQ(e.what(), std::string("It is impossible to represent a fraction with denominator 0"));
	}

	{
		rational num;

		ASSERT_EQ(num.numerator(), 0);
		ASSERT_EQ(num.denominator(), 1);

		try
		{
			auto k = num.inverse();
		}
		catch (const std::logic_error &e)
		{
			EXPECT_EQ(e.what(), std::string("There is no multiplication inverse for zero"));
		}
	}
	{
		rational num(101, 51);

		ASSERT_EQ(num.numerator(), 101);
		ASSERT_EQ(num.denominator(), 51);
	}
	{
		rational num(-101, 51);

		ASSERT_EQ(num.numerator(), -101);
		ASSERT_EQ(num.denominator(), 51);

		rational other = num.inverse();

		ASSERT_EQ(other.numerator(), -51);
		ASSERT_EQ(other.denominator(), 101);
	}
	{
		rational num(102, 51);

		ASSERT_EQ(num.numerator(), 2);
		ASSERT_EQ(num.denominator(), 1);
	}
	{
		rational num(210, 84);

		ASSERT_EQ(num.numerator(), 5);
		ASSERT_EQ(num.denominator(), 2);

		rational other = num.inverse();

		ASSERT_EQ(other.numerator(), 2);
		ASSERT_EQ(other.denominator(), 5);
	}
}

TEST (RationalTestSuite, TestPlus)
{
	using namespace big;

	{
		rational a(5, 7);
		rational b(7, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 124);
		ASSERT_EQ(c.denominator(), 105);
	}
	{
		rational a(4, 45);
		rational b(7, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 5);
		ASSERT_EQ(c.denominator(), 9);
	}
	{
		rational a(-4, 45);
		rational b(7, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 45);
	}
	{
		rational a(-4, 5);
		rational b(7, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), -1);
		ASSERT_EQ(c.denominator(), 3);
	}
	{
		rational a(4, 5);
		rational b(-7, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 3);
	}
	{
		rational a(4, 5);
		rational b(-74, 15);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), -62);
		ASSERT_EQ(c.denominator(), 15);
	}
	{
		rational a(4, 5);
		rational b(-4, 5);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 0);
		ASSERT_EQ(c.denominator(), 1);
	}
}

TEST (RationalTestSuite, TestMinus)
{
	using namespace big;

	{
		rational a(5, 7);
		rational b(7, 15);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 26);
		ASSERT_EQ(c.denominator(), 105);
	}
	{
		rational a(4, 30);
		rational b(7, 15);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), -1);
		ASSERT_EQ(c.denominator(), 3);
	}
	{
		rational a(-4, 30);
		rational b(7, 15);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), -3);
		ASSERT_EQ(c.denominator(), 5);
	}
	{
		rational a(-4, 30);
		rational b(-7, 15);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 3);
	}
	{
		rational a(4, 30);
		rational b(-7, 15);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 3);
		ASSERT_EQ(c.denominator(), 5);
	}
}

TEST (RationalTestSuite, TestMul)
{
	using namespace big;

	{
		rational a(17, 16);
		rational b(4, 7);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 28);
	}
	{
		rational a(17, 16);
		rational b(-12, 17);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), -3);
		ASSERT_EQ(c.denominator(), 4);
	}
	{
		rational a(-17, 16);
		rational b(0, 1);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 0);
		ASSERT_EQ(c.denominator(), 1);
	}
	{
		rational a(-17, 169);
		rational b(13, 54);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), -17);
		ASSERT_EQ(c.denominator(), 702);
	}
	{
		rational a(-17, 169);
		rational b(-13, 54);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 702);
	}
}

TEST (RationalTestSuite, TestDivide)
{
	using namespace big;

	try
	{
		rational a(15, 16);
		rational b(0, 1);

		a /= b;

		FAIL();
	}
	catch (const std::logic_error &e)
	{
		EXPECT_EQ(e.what(), std::string("There is no multiplication inverse for zero"));
	}
	{
		rational a(17, 169);
		rational b(13, 54);

		rational c = a / b;

		ASSERT_EQ(c.numerator(), 918);
		ASSERT_EQ(c.denominator(), 2197);
	}
	{
		rational a(136, 15);

		rational c = a / a.inverse();

		ASSERT_EQ(c.numerator(), 18496);
		ASSERT_EQ(c.denominator(), 225);
	}
	{
		rational a(-136, 15);

		rational c = a * a.inverse();

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 1);
	}
}