#include "../big/integer/integer.hpp"
#include "gtest/gtest.h"
#include "rational/rational.hpp"

TEST(IntegerTestSuite, TestComparison)
{
	using namespace big;

	EXPECT_TRUE(integer(-1) < integer(-0));
	EXPECT_TRUE(integer(-1) < integer(0));
	EXPECT_TRUE(integer(-10) < integer(0));
	EXPECT_TRUE(integer(0) == integer(-0));
	EXPECT_TRUE(integer(0) > integer(-1));
	EXPECT_TRUE(integer(1) > integer(-1));
	EXPECT_TRUE(integer(10) > integer(-1));
	EXPECT_TRUE(integer(5) > integer(-55555555));
	EXPECT_TRUE(integer(5) < integer(6));
	EXPECT_TRUE(integer(-5) < integer(6));
	EXPECT_TRUE(integer(-5) < integer(2));
}

TEST(IntegerTestSuite, TestPlus)
{
	using namespace big;

	EXPECT_EQ(integer(54) + integer(24), integer(78));
	EXPECT_EQ(integer(54) + integer(-24), integer(30));
	EXPECT_EQ(integer(54) + integer(-74), integer(-20));
	EXPECT_EQ(integer(-54) + integer(74), integer(20));
	EXPECT_EQ(integer(-54) + integer(24), integer(-30));
	EXPECT_EQ(integer(-54) + integer(-24), integer(-78));
}

TEST(IntegerTestSuite, TestMinus)
{
	using namespace big;

	EXPECT_EQ(integer(54) - integer(24), integer(30));
	EXPECT_EQ(integer(54) - integer(74), integer(-20));
	EXPECT_EQ(integer(54) - integer(-74), integer(128));
	EXPECT_EQ(integer(-54) - integer(74), integer(-128));
	EXPECT_EQ(integer(-10) - integer(55), integer(-65));
	EXPECT_EQ(integer(-10) - integer(-5), integer(-5));
	EXPECT_EQ(integer(-10) - integer(-55), integer(45));
}

TEST(IntegerTestSuite, TestProduct)
{
	using namespace big;

	integer a(999911205);

	EXPECT_EQ(a.to_str(), "999911205");

	a *= 554654684;

	EXPECT_EQ(a.to_str(), "554605433437334220");

	a *= 1;

	EXPECT_EQ(a.to_str(), "554605433437334220");

	a *= -1;

	EXPECT_EQ(a.to_str(), "-554605433437334220");

	a *= 0;

	EXPECT_EQ(a.to_str(), "0");

	a += 19999;
	a *= -55;

	EXPECT_EQ(a.to_str(), "-1099945");
}

TEST(IntegerTestSuite, TestBitwiseLeftShift)
{
	using namespace big;

	integer a(natural("554605433437334220"));

	a <<= 15;

	EXPECT_EQ(a.to_str(), "554605433437334220000000000000000");

	a *= -1;

	EXPECT_EQ(a.to_str(), "-554605433437334220000000000000000");

	a.flip_sing();

	EXPECT_EQ(a.to_str(), "554605433437334220000000000000000");

	a.flip_sing();

	std::string old = a.to_str();

	old += std::string(1500000, '0');

	a <<= 1500000;

	EXPECT_EQ(a.to_str(), old);

	a <<= 1500000;

	old += std::string(1500000, '0');

	EXPECT_EQ(a.to_str(), old);

	a <<= 0;

	EXPECT_EQ(a.to_str(), old);

	a *= -1;

	EXPECT_EQ(a.to_str(), old.substr(1));
}

TEST(IntegerTestSuite, TestBitwiseRightShift)
{
	using namespace big;

	std::string old = "554605433437334220";

	integer a((natural(old)));

	a >>= 0;

	EXPECT_EQ(a.to_str(), old);

	old.resize(old.size() - 14);

	a >>= 14;

	EXPECT_EQ(a.to_str(), old);

	a *= -1;

	EXPECT_EQ(a.to_str(), "-" + old);

	a >>= 7;

	EXPECT_EQ(a.to_str(), "0");
}

TEST(IntegerTestSuite, TestDivision)
{
	using namespace big;

	EXPECT_EQ(integer(42) / integer(7), integer(6));

	EXPECT_EQ(integer(56885154) / integer(7), integer(8126450));
	EXPECT_EQ(integer(56885154) / -integer(7), integer(-8126450));
	EXPECT_EQ(integer(-56885154) / integer(7), integer(-8126450));
	EXPECT_EQ(integer(-56885154) / integer(-7), integer(8126450));
	EXPECT_EQ(integer(-56885154) / -integer(555555555), integer(0));

	try
	{
		integer(554) / integer(0);
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("Cannot divide by zero"));
	}
}

TEST(IntegerTestSuite, TestModule)
{
	using namespace big;

	EXPECT_EQ(integer(15) % integer(7), integer(1));
	EXPECT_EQ(integer(18) % integer(7), integer(4));
	EXPECT_EQ(integer(-16) % integer(-7), integer(-2));
	EXPECT_EQ(integer(16) % integer(-7), integer(-5));
	EXPECT_EQ(integer(-16) % integer(7), integer(5));
	EXPECT_EQ(integer(-12) % integer(7), integer(2));
	EXPECT_EQ(integer(13) % integer(-7), integer(-1));
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
		EXPECT_EQ(e.what(), std::string("Cannot divide by zero"));
	}
	try
	{
		integer(554) % integer(-0);
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("Cannot divide by zero"));
	}
}

TEST(IntegerTestSuite, TestConstruct)
{
	using namespace big;

	{
		rational b(16, 5);
		integer a(b);

		ASSERT_EQ(a.to_str(), "3");
		ASSERT_EQ(a, 3);
	}
	{
		rational b(-165477446, 5395);
		integer a(b);

		ASSERT_EQ(a.to_str(), "-30672");
		ASSERT_EQ(a, -30672);
	}
	{
		rational b(-1654, 5395);
		integer a(b);

		ASSERT_EQ(a.to_str(), "0");
		ASSERT_EQ(a, 0);
	}
}