#include "../big/rational/rational.hpp"
#include "../big/numeric/rational.hpp"
#include "gtest/gtest.h"

TEST (RationalTestSuite, TestNumericRational)
{
	using namespace big;

	{
		natural a{5};

		ASSERT_EQ(numeric::rational::numerator(a), 5);
		ASSERT_EQ(numeric::rational::denominator(a), 1);
	}
	{
		integer num(-15);

		ASSERT_EQ(numeric::rational::numerator(num), -15);
		ASSERT_EQ(numeric::rational::denominator(num), 1);
	}
	{
		rational num(-15);

		ASSERT_EQ(numeric::rational::numerator(num), -15);
		ASSERT_EQ(numeric::rational::denominator(num), 1);
	}
	{
		rational num(-16, 7);

		ASSERT_EQ(numeric::rational::numerator(num), -16);
		ASSERT_EQ(numeric::rational::denominator(num), 7);
	}
}

TEST (RationalTestSuite, TestCreation)
{
	using namespace big;

	try
	{
		rational a(1, 0u);

		FAIL();
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("it is impossible to represent a fraction with denominator 0"));
	}

	{
		rational num{};

		ASSERT_EQ(num.numerator(), 0);
		ASSERT_EQ(num.denominator(), 1u);

		try
		{
			auto k = num.inverse();
		}
		catch (const std::logic_error &e)
		{
			EXPECT_EQ(e.what(), std::string("it is impossible to represent a fraction with denominator 0"));
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
		rational a(5, 7u);
		natural b(7);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), 54);
		ASSERT_EQ(c.denominator(), 7);
	}
	{
		rational a(5, 7u);
		integer b(-7);

		rational c = a + b;

		ASSERT_EQ(c.numerator(), -44);
		ASSERT_EQ(c.denominator(), 7);
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
		rational a(7, 15u);
		natural b(5);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), -68);
		ASSERT_EQ(c.denominator(), 15);
	}
	{
		rational a(7, 15u);
		integer b(-5);

		rational c = a - b;

		ASSERT_EQ(c.numerator(), 82);
		ASSERT_EQ(c.denominator(), 15);
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
		rational a(-1758, 15756);
		integer b(101);

		rational c = a * b;

		ASSERT_EQ(c.numerator(), -293);
		ASSERT_EQ(c.denominator(), 26);
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
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("it is impossible to represent a fraction with denominator 0"));
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

		rational c = a / a.inverse();

		ASSERT_EQ(c.numerator(), 18496);
		ASSERT_EQ(c.denominator(), 225);
	}
	{
		rational a(-136, 17u);
		rational b(184, 6);

		rational c = a / b;

		ASSERT_EQ(c.numerator(), -6);
		ASSERT_EQ(c.denominator(), 23);
	}
	{
		rational a(136, 15u);
		rational b(-184, 6);

		rational c = a / b;

		ASSERT_EQ(c.numerator(), -34);
		ASSERT_EQ(c.denominator(), 115);
	}
	{
		rational a(136, 23u);

		rational c = a / -1;

		ASSERT_EQ(c.numerator(), -136);
		ASSERT_EQ(c.denominator(), 23);
	}
	{
		rational a(natural("471283764823694817263984712693874691283746981237649124412341234"), natural("34123"));

		rational c = a / -2;

		ASSERT_EQ(c.numerator(), -integer(natural("235641882411847408631992356346937345641873490618824562206170617")));
		ASSERT_EQ(c.denominator(), 34123);
	}
}
