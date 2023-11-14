#include "../big/natural/natural.hpp"
#include "../big/algorithm/algorithm.hpp"
#include "gtest/gtest.h"

TEST(NaturalTestSuite, TestConstruction)
{
	using namespace big;

	// from integer
	ASSERT_EQ(natural(1u).str(), "1");
	ASSERT_EQ(natural(572275u).str(), "572275");
	ASSERT_EQ(natural(1000000007u).str(), "1000000007");
	ASSERT_EQ(natural(1000000000u).str(), "1000000000");
	ASSERT_EQ(natural(18446744073709551615ull).str(), "18446744073709551615");

	// from string
	ASSERT_EQ(natural("0").str(), "0");
	ASSERT_EQ(natural("1").str(), "1");
	ASSERT_EQ(natural("57").str(), "57");
	ASSERT_EQ(natural("57558858585858").str(), "57558858585858");
	ASSERT_EQ(natural("5464841321654684321354687465132146874651354984651354687465432135468798465132165487654321654798462168465468324792").str(), "5464841321654684321354687465132146874651354984651354687465432135468798465132165487654321654798462168465468324792");
	ASSERT_EQ(natural("999911205").str(), "999911205");

	// exceptions
	{
		try
		{
			natural a("");
			FAIL();
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_EQ(e.what(), std::string("cannot build num from empty string"));
		}
		catch (...)
		{
			EXPECT_FALSE("unknown exception");
		}
	}
	{
		try
		{
			natural a("55 6");
			FAIL();
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_EQ(e.what(), std::string("invalid number, one or more characters are not a digit"));
		}
		catch (...)
		{
			EXPECT_FALSE("unknown exception");
		}
	}
	{
		try
		{
			natural a("-556");
			FAIL();
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_EQ(e.what(), std::string("invalid number, one or more characters are not a digit"));
		}
		catch (...)
		{
			EXPECT_FALSE("unknown exception");
		}
	}
	{
		try
		{
			natural a("54738461283746127364686712348723f56");
			FAIL();
		}
		catch (const std::invalid_argument &e)
		{
			EXPECT_EQ(e.what(), std::string("invalid number, one or more characters are not a digit"));
		}
		catch (...)
		{
			EXPECT_FALSE("unknown exception");
		}
	}
}

TEST(NaturalTestSuite, TestComparison)
{
	using namespace big;

	ASSERT_TRUE(natural("1100") != natural("10001"));
	ASSERT_TRUE(natural("555") == natural("555"));
	ASSERT_FALSE(natural("555") < natural("555"));
	ASSERT_TRUE(natural("555") <= natural("555"));
	ASSERT_TRUE(natural("555") < natural("556"));
	ASSERT_TRUE(natural("555") < natural("1555"));
	ASSERT_TRUE(natural("555857857877") > natural("555857857867"));
	ASSERT_TRUE(natural("1000000000000") > natural("555857857867"));
	ASSERT_TRUE(natural("1000000000000") >= natural("555857857867"));
	ASSERT_TRUE(natural("1000000000000") >= natural("1000000000000"));
}

TEST(NaturalTestSuite, TestPlus)
{
	using namespace big;

	{
		natural num(8589934586u);
		ASSERT_EQ(num.str(), "8589934586");
		num += num;
		ASSERT_EQ(num.str(), "17179869172");
	}
	{
		natural num("999999999999999999999999999");
		ASSERT_EQ(num.str(), "999999999999999999999999999");
		num += natural("999999999999999999999999999");
		ASSERT_EQ(num.str(), "1999999999999999999999999998");
	}
	{
		natural a(0u);

		ASSERT_EQ(a.str(), "0");
		a += 3u;
		ASSERT_EQ(a.str(), "3");
		a <<= 1u;
		ASSERT_EQ(a.str(), "3000000000");
		a += 10u;
		ASSERT_EQ(a.str(), "3000000010");
		a += 7u;
		ASSERT_EQ(a.str(), "3000000017");
		a += 99999999999u;
		ASSERT_EQ(a.str(), "103000000016");
		a >>= 1;
		ASSERT_EQ(a.str(), "103");
		a += natural("5464841321654684321354687465132146874651354984651354687465432135468798465132165487654321654798462168465468324689");
		ASSERT_EQ(a.str(), "5464841321654684321354687465132146874651354984651354687465432135468798465132165487654321654798462168465468324792");
		a += a;
		ASSERT_EQ(a.str(), "10929682643309368642709374930264293749302709969302709374930864270937596930264330975308643309596924336930936649584");
		a *= 0u;
		ASSERT_EQ(a.str(), "0");
		a += static_cast<uint32_t>(1e9 + 7);
		ASSERT_EQ(a.str(), "1000000007");
	}
}

TEST(NaturalTestSuite, TestMinus)
{
	using namespace big;

	{
		natural num(8589934586u);
		ASSERT_EQ(num.str(), "8589934586");
		num -= num;
		ASSERT_EQ(num.str(), "0");
	}
	{

		natural a(103000000016u);

		ASSERT_EQ(a.str(), "103000000016");
		--a;
		ASSERT_EQ(a.str(), "103000000015");
		a -= 150000u;
		ASSERT_EQ(a.str(), "102999850015");
		a -= 7998500096u;
		ASSERT_EQ(a.str(), "95001349919");
	}
	{
		natural a("41238749812376498394761293846981237649812736481726308412341234");

		ASSERT_EQ(a.str(), "41238749812376498394761293846981237649812736481726308412341234");
		a -= natural("98123764983947612938469812376498127364817263468465");
		ASSERT_EQ(a.str(), "41238749812278374629777346234042767837436238354361491148872769");
	}
	{
		try
		{
			natural a(15u);
			natural b(16u);

			a = a - b;
			--b;
			a += b;
			a -= b;
		}
		catch (const std::domain_error &e)
		{
			EXPECT_EQ(e.what(), std::string("it is impossible to subtract a larger natural number"));
		}
		catch (...)
		{
			EXPECT_FALSE("unknown exception");
		}
	}
}

TEST(NaturalTestSuite, TestProduct)
{
	using namespace big;

	natural a("999911205");

	ASSERT_EQ(a.str(), "999911205");

	a *= natural("554654684");

	ASSERT_EQ(a.str(), "554605433437334220");

	a *= 1u;

	ASSERT_EQ(a.str(), "554605433437334220");

	a *= 0u;

	ASSERT_EQ(a.str(), "0");

	a += 554605433437u;

	ASSERT_EQ(a.str(), "554605433437");

	a *= 554605433437u;
	a *= a;
	a *= a;

	ASSERT_EQ(a.str(), "8951028917198964712757504215998227867810191445839800441149608111417415741073044384885876111521");
}

TEST(NaturalTestSuite, TestBitwiseLeftShift)
{
	using namespace big;

	std::string initial = "554605433437334220";

	natural a(initial);

	a <<= 15;

	initial += std::string(9 * 15, '0');

	ASSERT_EQ(a.str(), initial);

	a <<= 0;

	ASSERT_EQ(a.str(), initial);

	a <<= 1500000;

	initial += std::string(9 * 1500000, '0');

	ASSERT_EQ(a.str(), initial);
}

TEST(NaturalTestSuite, TestBitwiseRightShift)
{
	using namespace big;

	std::string old = "554605433437334220";

	natural a(old);

	a >>= 0;

	ASSERT_EQ(a.str(), old);

	old = old.substr(0, 9);

	a >>= 1;

	ASSERT_EQ(a.str(), old);

	a >>= 7;

	ASSERT_EQ(a.str(), "0");
}

TEST(NaturalTestSuite, TestDivision)
{
	using namespace big;

	ASSERT_EQ(natural(42).long_div(natural(7)).first, natural(6));
	ASSERT_EQ(natural(42).long_div(natural(7)).second, natural(0));

	ASSERT_EQ(natural("56885154").long_div(natural("7")).first, natural("8126450"));
	ASSERT_EQ(natural("56885154").long_div(natural("7")).second, natural("4"));

	ASSERT_EQ(natural("56885154").long_div(natural("9954984651")).first, natural("0"));
	ASSERT_EQ(natural("56885154").long_div(natural("9954984651")).second, natural("56885154"));

	ASSERT_EQ(natural("2221249360000000000000000000000000000000").long_div(natural("2")).first, natural("1110624680000000000000000000000000000000"));

	ASSERT_EQ(natural("22212493600000000000000000000000000000000000000000000000000000000004612837400000000000000000000000000000000000000000000000000000000041263547162384671238412").long_div(natural("2")).first, natural("11106246800000000000000000000000000000000000000000000000000000000002306418700000000000000000000000000000000000000000000000000000000020631773581192335619206"));
	ASSERT_EQ(natural("22212493600000000000000000000000000000000000000000000000000000000004612837400000000000000000000000000000000000000000000000000000000041263547162384671238412").long_div(natural("220000000000000000000041234000000000000")).first, natural("100965879999999999999981076240473090909090912637737728784406611569604128908596835353538813378857195082231964457132437"));

	ASSERT_EQ(natural("84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987").long_div(natural("2")).first, natural("42293567293581749288491587329486824937814187408619923581469230641873368810692460881992306418734911882493"));
	ASSERT_EQ(natural("84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987").long_div(natural("2")).second, natural("1"));

	std::string very_large = "84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987348136498172639846192837649387162398463981273649823948738649369812763948712639846739756483748912736947126398461273649712346936498276398";
	std::string divisor = "431289124387461298346192837";
	std::string quotient = "196126286994364719427352371814946481139567206978944028924245223561914598598342572620318055970420645044007842319309020510965457372840772412270676823713153740039602065340067348480364188933227621073696228858028328581";
	std::string remainder = "277023148385491500673702101";

	auto ret = natural(very_large).long_div(natural(divisor));
	ASSERT_EQ(ret.first.str(), quotient);
	ASSERT_EQ(ret.second.str(), remainder);

	natural num(very_large);

	num <<= 17;

	ASSERT_EQ((num / natural(divisor)).str(), "196126286994364719427352371814946481139567206978944028924245223561914598598342572620318055970420645044007842319309020510965457372840772412270676823713153740039602065340067348480364188933227621073696228858028328581642314245180501211270540331561880337515765283242188812515809136679792656415975503700621773860312954111030218228391060091874645009574680934122993774116829");

	natural k = num;

	num <<= 150000;

	num /= k;

	try
	{
		std::ignore = natural("554") / natural("0");
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("division by zero"));
	}
}

TEST(NaturalTestSuite, TestModule)
{
	using namespace big;

	natural num("29");
	natural base = num;

	// 29 ^ 183

	for (int i = 0; i < 182; ++i)
	{
		base *= num;
	}

	// 29 ^ 183 % 73 == 66
	ASSERT_EQ(base % natural(73u), natural("66"));

	ASSERT_EQ((base * base * base) % natural("154654846"), natural("6396741"));

	try
	{
		std::ignore = natural("554") % natural("0");
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("division by zero"));
	}

	ASSERT_EQ(natural("42") % natural("6"), natural("0"));
}
