#include "../big/integer/integer.hpp"
#include "../big/rational/rational.hpp"
#include "gtest/gtest.h"

TEST(IntegerTestSuite, TestComparison)
{
	using namespace big;

	ASSERT_TRUE(integer(-1) < integer(-0));
	ASSERT_TRUE(integer(-1) < integer(0));
	ASSERT_TRUE(integer(-10) < integer(0));
	ASSERT_TRUE(integer(0) == integer(-0));
	ASSERT_TRUE(integer(0) > integer(-1));
	ASSERT_TRUE(integer(1) > integer(-1));
	ASSERT_TRUE(integer(10) > integer(-1));
	ASSERT_TRUE(integer(5) > integer(-55555555));
	ASSERT_TRUE(integer(5) < integer(6));
	ASSERT_TRUE(integer(-5) < integer(6));
	ASSERT_TRUE(integer(-5) < integer(2));
	ASSERT_TRUE(integer(natural("4128374698239487129387492183741234")) > natural("4128374698239487129387492183741233"));
}

TEST(IntegerTestSuite, TestPlus)
{
	using namespace big;

	ASSERT_EQ(integer(54) + integer(24), integer(78));
	ASSERT_EQ(integer(54) + integer(-24), integer(30));
	ASSERT_EQ(integer(54) + integer(-74), integer(-20));
	ASSERT_EQ(integer(-54) + integer(74), integer(20));
	ASSERT_EQ(integer(-54) + integer(24), integer(-30));
	ASSERT_EQ(integer(-54) + integer(-24), integer(-78));
	ASSERT_EQ(integer(-54) + natural(74), integer(20));
}

TEST(IntegerTestSuite, TestMinus)
{
	using namespace big;

	ASSERT_EQ(integer(54) - integer(24), integer(30));
	ASSERT_EQ(integer(54) - integer(74), integer(-20));
	ASSERT_EQ(integer(54) - integer(-74), integer(128));
	ASSERT_EQ(integer(-54) - integer(74), integer(-128));
	ASSERT_EQ(integer(-10) - integer(55), integer(-65));
	ASSERT_EQ(integer(-10) - integer(-5), integer(-5));
	ASSERT_EQ(integer(-10) - integer(-55), integer(45));
	ASSERT_EQ(integer(-10) - natural(55), integer(-65));
}

TEST(IntegerTestSuite, TestProduct)
{
	using namespace big;

	integer a(999911205);

	ASSERT_EQ(a.str(), "999911205");

	a *= 554654684;

	ASSERT_EQ(a.str(), "554605433437334220");

	a *= 1;

	ASSERT_EQ(a.str(), "554605433437334220");

	a *= -1;

	ASSERT_EQ(a.str(), "-554605433437334220");

	a *= 0;

	ASSERT_EQ(a.str(), "0");

	a += 19999;
	a *= -55;

	ASSERT_EQ(a.str(), "-1099945");

	a *= integer(natural("123846712384681927354761253876489737419276398471265936518237640812734987387412387694127312374812736498123694871234"), true);

	ASSERT_EQ(a.str(), "136224572053968962584232867395175504225645978116476610543552901823763785701847318782216866610113395447428667555139482130");

	a += natural("142374123412");

	ASSERT_EQ(a.str(), "136224572053968962584232867395175504225645978116476610543552901823763785701847318782216866610113395447428667697513605542");
}

TEST(IntegerTestSuite, TestBitwiseLeftShift)
{
	using namespace big;

	integer a(natural("554605433437334220"));

	a <<= 1;

	ASSERT_EQ(a.str(), "554605433437334220000000000");

	a *= -1;

	ASSERT_EQ(a.str(), "-554605433437334220000000000");

	a.flip_sign();

	ASSERT_EQ(a.str(), "554605433437334220000000000");

	a.flip_sign();

	std::string old = a.str();

	old += std::string(150000 * 9, '0');

	a <<= 150000;

	ASSERT_EQ(a.str(), old);
}

TEST(IntegerTestSuite, TestBitwiseRightShift)
{
	using namespace big;

	std::string old = "554605433437334220";

	integer a(natural(old), true);

	a >>= 0;

	old = "-" + old;

	ASSERT_EQ(a.str(), old);

	old = old.substr(0, 10);

	a >>= 1;

	ASSERT_EQ(a.str(), old);

	a *= -1;

	ASSERT_EQ(a.str(), old.substr(1));

	a >>= 1;

	ASSERT_EQ(a.str(), "0");
}

TEST(IntegerTestSuite, TestDivision)
{
	using namespace big;

	ASSERT_EQ(integer(42) / integer(7), integer(6));

	ASSERT_EQ(integer(56885154) / integer(7), integer(8126450));
	ASSERT_EQ(integer(56885154) / -integer(7), integer(-8126450));
	ASSERT_EQ(integer(-56885154) / integer(7), integer(-8126450));
	ASSERT_EQ(integer(-56885154) / integer(-7), integer(8126450));
	ASSERT_EQ(integer(-56885154) / -integer(555555555), integer(0));
	ASSERT_EQ(integer(natural("471283764823694817263984712693874691283746981237649124412341234")) / -integer(34123), integer(natural("13811322709717633773817797752069709324612343030731445781799"), true));

	try
	{
		integer(554) / integer(0);
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("division by zero"));
	}
}

TEST(IntegerTestSuite, TestModule)
{
	using namespace big;

	EXPECT_EQ(integer(15) % integer(7), integer(1));
	EXPECT_EQ(integer(18) % integer(7), integer(4));
	EXPECT_EQ(integer(-16) % integer(-7), integer(-2));
	EXPECT_EQ(integer(16) % integer(-7), integer(2));
	EXPECT_EQ(integer(-16) % integer(7), integer(-2));
	EXPECT_EQ(integer(-12) % integer(7), integer(-5));
	EXPECT_EQ(integer(13) % integer(-7), integer(6));
	EXPECT_EQ(integer(-21) % integer(-7), integer(0));
	EXPECT_EQ(integer(21) % integer(-7), integer(0));
	EXPECT_EQ(integer(-21) % integer(7), integer(0));
	EXPECT_EQ(integer(-2222) % integer(-3), integer(-2));

	try
	{
		integer(554) % integer(0);
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("division by zero"));
	}
	try
	{
		integer(554) % integer(-0);
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("division by zero"));
	}
}
