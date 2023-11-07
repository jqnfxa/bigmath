#include "../big/rational/rational.hpp"
#include "gtest/gtest.h"

TEST (RationalTestSuite, TestCreation)
{
	using namespace big;

	try
	{
		rational a(1, 0u);

		FAIL();
	}
	catch (const std::invalid_argument &e)
	{
		EXPECT_EQ(e.what(), std::string("It is impossible to represent a fraction with denominator 0"));
	}

	{
		rational num;

		ASSERT_EQ(num.numerator(), 0);
		ASSERT_EQ(num.denominator(), 1u);

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
		rational num(101, 51u);

		ASSERT_EQ(num.numerator(), 101);
		ASSERT_EQ(num.denominator(), 51u);
	}
	{
		rational num(-101, 51u);

		ASSERT_EQ(num.numerator(), -101);
		ASSERT_EQ(num.denominator(), 51u);

		rational other = num.inverse();

		ASSERT_EQ(other.numerator(), -51);
		ASSERT_EQ(other.denominator(), 101u);
	}
	{
		rational num(102, 51u);

		ASSERT_EQ(num.numerator(), 2);
		ASSERT_EQ(num.denominator(), 1u);
	}
	{
		rational num(210, 84u);

		ASSERT_EQ(num.numerator(), 5);
		ASSERT_EQ(num.denominator(), 2u);

		rational other = num.inverse();

		ASSERT_EQ(other.numerator(), 2);
		ASSERT_EQ(other.denominator(), 5u);
	}
}

TEST (RationalTestSuite, TestPlus)
{
	using namespace big;

	{
		rational a(5, 7u);
		rational b(7, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 124);
		ASSERT_EQ(c.denominator(), 105u);
	}
	{
		rational a(4, 45u);
		rational b(7, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 5);
		ASSERT_EQ(c.denominator(), 9u);
	}
	{
		rational a(-4, 45u);
		rational b(7, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 45u);
	}
	{
		rational a(-4, 5u);
		rational b(7, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), -1);
		ASSERT_EQ(c.denominator(), 3u);
	}
	{
		rational a(4, 5u);
		rational b(-7, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 3u);
	}
	{
		rational a(4, 5u);
		rational b(-74, 15u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), -62);
		ASSERT_EQ(c.denominator(), 15u);
	}
	{
		rational a(4, 5u);
		rational b(-4, 5u);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 0);
		ASSERT_EQ(c.denominator(), 1u);
	}
}

TEST (RationalTestSuite, TestMinus)
{
	using namespace big;

	{
		rational a(5, 7u);
		rational b(7, 15u);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 26);
		ASSERT_EQ(c.denominator(), 105u);
	}
	{
		rational a(4, 30u);
		rational b(7, 15u);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), -1);
		ASSERT_EQ(c.denominator(), 3u);
	}
	{
		rational a(-4, 30u);
		rational b(7, 15u);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), -3);
		ASSERT_EQ(c.denominator(), 5u);
	}
	{
		rational a(-4, 30u);
		rational b(-7, 15u);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 3u);
	}
	{
		rational a(4, 30u);
		rational b(-7, 15u);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 3);
		ASSERT_EQ(c.denominator(), 5u);
	}
}

TEST (RationalTestSuite, TestMul)
{
	using namespace big;

	{
		rational a(17, 16u);
		rational b(4, 7u);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 28u);
	}
	{
		rational a(17, 16u);
		rational b(-12, 17u);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), -3);
		ASSERT_EQ(c.denominator(), 4u);
	}
	{
		rational a(-17, 16u);
		rational b(0, 1u);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 0);
		ASSERT_EQ(c.denominator(), 1u);
	}
	{
		rational a(-17, 169u);
		rational b(13, 54u);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), -17);
		ASSERT_EQ(c.denominator(), 702u);
	}
	{
		rational a(-17, 169u);
		rational b(-13, 54u);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), 17);
		ASSERT_EQ(c.denominator(), 702u);
	}
}

TEST (RationalTestSuite, TestDivide)
{
	using namespace big;

	try
	{
		rational a(15, 16u);
		rational b(0, 1u);

		a /= b;

		FAIL();
	}
	catch (const std::logic_error &e)
	{
		EXPECT_EQ(e.what(), std::string("There is no multiplication inverse for zero"));
	}
	{
		rational a(17, 169u);
		rational b(13, 54u);

		rational c = a / b;

		ASSERT_EQ(c.numerator(), 918);
		ASSERT_EQ(c.denominator(), 2197u);
	}
	{
		rational a(136, 15u);

		rational c = a / a.inverse();

		ASSERT_EQ(c.numerator(), 18496);
		ASSERT_EQ(c.denominator(), 225u);
	}
	{
		rational a(-136, 15u);

		rational c = a * a.inverse();

		ASSERT_EQ(c.numerator(), 1);
		ASSERT_EQ(c.denominator(), 1u);
	}
}