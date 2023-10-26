#include "../big/integer/integer.hpp"
#include "gtest/gtest.h"

TEST(IntegerTestSuite, TestComparison)
{
	using namespace big;

	EXPECT_TRUE(integer(-1) < integer(-0));
	EXPECT_TRUE(integer(-1) < integer(0));
	EXPECT_TRUE(integer(0) == integer(-0));
	EXPECT_TRUE(integer(0) > integer(-1));
	EXPECT_TRUE(integer(1) >= integer(0));
	EXPECT_TRUE(integer(5) > integer(-55555555));
	EXPECT_TRUE(integer(5) < integer(6));
	EXPECT_TRUE(integer(-5) < integer(6));
}

TEST(IntegerTestSuite, TestPlus)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestMinus)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestProduct)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestBitwiseLeftShift)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestBitwiseRightShift)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestDivision)
{
	using namespace big;

	// TODO implement
	FAIL();
}

TEST(IntegerTestSuite, TestModule)
{
	using namespace big;

	// TODO implement
	FAIL();
}