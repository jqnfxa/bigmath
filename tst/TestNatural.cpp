#include "../big/natural/natural.hpp"
#include "../big/algorithm/algorithm.hpp"
#include "gtest/gtest.h"

TEST(NaturalTestSuite, TestComparison)
{
	using namespace big;

	//EXPECT_TRUE(natural("lpplpPP") == natural("0"));
	EXPECT_TRUE(natural("1100") != natural("10001"));
	EXPECT_TRUE(natural("555") == natural("555"));
	EXPECT_FALSE(natural("555") < natural("555"));
	EXPECT_TRUE(natural("555") <= natural("555"));
	EXPECT_TRUE(natural("555") < natural("556"));
	EXPECT_TRUE(natural("555") < natural("1555"));
	EXPECT_TRUE(natural("555857857877") > natural("555857857867"));
	EXPECT_TRUE(natural("1000000000000") > natural("555857857867"));
	EXPECT_TRUE(natural("1000000000000") >= natural("555857857867"));
	EXPECT_TRUE(natural("1000000000000") >= natural("1000000000000"));
}

TEST(NaturalTestSuite, TestPlus)
{
	using namespace big;

	natural a(0);

	EXPECT_EQ(a.to_str(), "0");

	a += natural(3);
	EXPECT_EQ(a.to_str(), "3");
	a <<= 1;
	EXPECT_EQ(a.to_str(), "30");
	a += natural(10);
	EXPECT_EQ(a.to_str(), "40");
	a += natural(7);
	EXPECT_EQ(a.to_str(), "47");
	a += natural(99999999999);
	EXPECT_EQ(a.to_str(), "100000000046");
	a >>= 2;
	EXPECT_EQ(a.to_str(), "1000000000");
}

TEST(NaturalTestSuite, TestMinus)
{
	using namespace big;

	natural a(1000000000);

	a -= natural(999911205);

	EXPECT_EQ(a.to_str(), "88795");

	a -= natural();

	EXPECT_EQ(a.to_str(), "88795");

	a -= natural(88789);

	EXPECT_EQ(a.to_str(), "6");

	a -= natural(5);

	EXPECT_EQ(a.to_str(), "1");

	--a;

	EXPECT_EQ(a.to_str(), "0");

	try
	{
		natural("1") - natural("2");
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("Unable to subtract the greater natural"));
	}
}

TEST(NaturalTestSuite, TestProduct)
{
	using namespace big;

	natural a("999911205");

	EXPECT_EQ(a.to_str(), "999911205");

	a *= natural("554654684");

	EXPECT_EQ(a.to_str(), "554605433437334220");

	a *= natural(1);

	EXPECT_EQ(a.to_str(), "554605433437334220");

	a *= natural(0);

	EXPECT_EQ(a.to_str(), "0");

	a += natural(554605433437);

	EXPECT_EQ(a.to_str(), "554605433437");

	a *= natural(554605433437);
	a *= a;
	a *= a;

	EXPECT_EQ(a.to_str(), "8951028917198964712757504215998227867810191445839800441149608111417415741073044384885876111521");
}

TEST(NaturalTestSuite, TestBitwiseLeftShift)
{
	using namespace big;

	natural a("554605433437334220");

	a <<= 15;

	EXPECT_EQ(a.to_str(), "554605433437334220000000000000000");

	std::string old = a.to_str();

	old += std::string(1500000, '0');

	a <<= 1500000;

	EXPECT_EQ(a.to_str(), old);

	a <<= 1500000;

	old += std::string(1500000, '0');

	EXPECT_EQ(a.to_str(), old);

	a <<= 0;

	EXPECT_EQ(a.to_str(), old);
}

TEST(NaturalTestSuite, TestBitwiseRightShift)
{
	using namespace big;

	std::string old = "554605433437334220";

	natural a(old);

	a >>= 0;

	EXPECT_EQ(a.to_str(), old);

	old.resize(old.size() - 14);

	a >>= 14;

	EXPECT_EQ(a.to_str(), old);

	a >>= 7;

	EXPECT_EQ(a.to_str(), "0");
}

TEST(NaturalTestSuite, TestDivision)
{
	using namespace big;

	EXPECT_EQ(natural("42").long_div(natural("7")).first, natural("6"));
	EXPECT_EQ(natural("42").long_div(natural("7")).second, natural("0"));

	EXPECT_EQ(natural("56885154").long_div(natural("7")).first, natural("8126450"));
	EXPECT_EQ(natural("56885154").long_div(natural("7")).second, natural("4"));

	EXPECT_EQ(natural("56885154").long_div(natural("9954984651")).first, natural("0"));
	EXPECT_EQ(natural("56885154").long_div(natural("9954984651")).second, natural("56885154"));

	EXPECT_EQ(natural("84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987").long_div(natural("2")).first, natural("42293567293581749288491587329486824937814187408619923581469230641873368810692460881992306418734911882493"));
	EXPECT_EQ(natural("84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987").long_div(natural("2")).second, natural("1"));

	std::string very_large = "84587134587163498576983174658973649875628374817239847162938461283746737621384921763984612837469823764987348136498172639846192837649387162398463981273649823948738649369812763948712639846739756483748912736947126398461273649712346936498276398";
	std::string divisor = "431289124387461298346192837";
	std::string quotient = "196126286994364719427352371814946481139567206978944028924245223561914598598342572620318055970420645044007842319309020510965457372840772412270676823713153740039602065340067348480364188933227621073696228858028328581";
	std::string remainder = "277023148385491500673702101";

	auto ret = natural(very_large).long_div(natural(divisor));
	EXPECT_EQ(ret.first.to_str(), quotient);
	EXPECT_EQ(ret.second.to_str(), remainder);

	natural num(very_large);

	num <<= 135;
	num *= num;

	EXPECT_EQ((num
			   / natural(divisor)).to_str(), "16589760634072982601598520990767007822486366031367668643748451473469680852342754074368303333454281540413703962441843788126425597574131364741253047276626556264855769857882493686709012911189329624723299799927347321062828898577177385023842335657566400810794362469978716317611594605697246549479252503491640377452404768088516464715159554085409133663279299851442314425945275780836071221670870234065894007842746561021631634608091164680951943504416901881885720475066136388214293300796818833427667373259162310629933432983391108750072158732127782487797514015741018297318003687991841386346818555789929453008257024946059792201007621599405550030758917947018587114844138613678175246619009352025844341439312335204191479892501702655679226");

	try
	{
		natural("554") / natural("0");
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("Cannot divide by zero"));
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
	EXPECT_EQ(base % natural(73), natural("66"));

	EXPECT_EQ((base * base * base) % natural("154654846"), natural("6396741"));

	try
	{
		natural("554") % natural("0");
	}
	catch (const std::domain_error &e)
	{
		EXPECT_EQ(e.what(), std::string("Cannot divide by zero"));
	}

	EXPECT_EQ(natural("42") % natural("6"), natural("0"));
}