#include <valarray>
#include <chrono>
#include "../big/algorithm/algorithm.hpp"
#include "../big/natural/natural.hpp"
#include "../big/rational/rational.hpp"
#include "../big/polynomial/polynomial.hpp"
#include "gtest/gtest.h"

TEST(AlgorithmTestSuite, TestGcd)
{
	using namespace big;

	EXPECT_EQ(gcd(natural("48123749817263487162398476123987461293846391"), natural("714263874612")), natural("3"));
	EXPECT_EQ(gcd(natural("34542396282240"), natural("154")), natural("154"));
}

TEST(AlgorithmTestSuite, TestLcm)
{
	using namespace big;

	EXPECT_EQ(lcm(natural("12265103118755758026325601433600"), natural("565646")), natural("3468853259355859752279485574255052800"));
}

TEST(AlgorithmTestSuite, PolynomialGcd)
{
	using namespace big;

	{
		std::vector<rational> p1 = {rational(1), rational(2), rational(3), rational(6)};
		std::vector<rational> p2 = {rational(1), rational(2)};
		std::vector<rational> test = {rational(1), rational(2)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(0), rational(1), rational(0), rational(-3),
									rational(-3), rational(8), rational(2), rational(-5)};
		std::vector<rational> p2 = {rational(3), rational(0), rational(5), rational(0), rational(-4),
									rational(-9), rational(21)};
		std::vector<rational> test = {rational(-1288744821, 543589225u)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(5), rational(6), rational(9), rational(2)};
		std::vector<rational> p2 = {rational(1), rational(3), rational(1), rational(-2), rational(-5)};
		std::vector<rational> test = {rational(-335915, 87616u)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(1), rational(3), rational(3)};
		std::vector<rational> p2 = {rational(1), rational(1)};
		std::vector<rational> test = {rational(1), rational(1)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
	{
		std::vector<rational> p1 = {rational(1), rational(2), rational(3), rational(6)};
		std::vector<rational> p2 = {rational(1), rational(2)};
		std::vector<rational> test = {rational(1), rational(2)};

		polynomial cmp = gcd(polynomial(p1), polynomial(p2));

		auto exp = test;
		std::reverse(exp.begin(), exp.end());

		ASSERT_TRUE(std::equal(cmp.coefficients().begin(), cmp.coefficients().end(), exp.begin(), exp.end()));
	}
}

TEST(AlgorithmTestSuite, TestNumsPower)
{
	using namespace big;

	{
		rational a(1);

		a = big::pow(a, integer(natural("10000000000000")));

		ASSERT_EQ(a, rational(1));

		a *= integer(-1);

		a = big::pow(a, integer(natural("1001")));

		ASSERT_EQ(a, rational(-1));

		a = big::pow(a, 2);

		ASSERT_EQ(a, rational(1));
	}
	{
		rational num(-138, 967u);

		num = big::pow(num, 0);

		ASSERT_EQ(num, rational(1));
	}
	{
		rational num(-138, 967u);

		num = big::pow(num, 5);

		rational exp(-integer(natural("50049003168")), natural("845536520469607"));

		ASSERT_EQ(num, exp);
	}
	{
		rational num(-138, 7u);

		num = big::pow(num, -5);

		rational exp(-integer(natural("16807")), natural("50049003168"));

		ASSERT_EQ(num, exp);
	}
	{
		ASSERT_EQ(integer(big::pow(integer(39), 67)), integer(natural("39932699568340257292773590640099220617455013974544034477685615256178430466321637305610616222669155932785079")));
	}
	{
		integer num = big::pow(big::pow(integer(39), 67), 15);
		ASSERT_EQ(num, integer(natural("1046959933047457409037646687208630588472606285961837842099741301002543221696236750588045586436630210715491570226637914183814561173203244762292058303809091362981361427954730273223662256584223850800848949128708423511330152898707757008658666303154614973857458143029532084059317225122033615403938976959887097271086853809563931661982486060604814217229066167474462673194696277303007911720185583829510500949004297997524352832100079529703626516907657089257685815111880828748560027928095955114167232941918467095098321278253417324726610350946007451097629579076556435309894385666917667910056469562994066555691996692847135540132342254526531697760793199003703610206664291893528953082980173661228196292379132082007963597855612324716065350812126872248789968588612325019496968827624931934134908015261058786526549945575828388834793896455174843750062710566473626150066786099084807052406436005959241160156024466739334566343845101186016696061886421246204481628023524260195720876523855953060214883508881159774260887012741761910921671083453289760141130339411513497107572342025832677247380377242864288526012100200693354821133559541471860421333951556834573483982101313812084520738435732982520371216539479437200143045874969923979474157972590935125523435340973645755147378796530350864078404055293544811238539442466414086732269137867729132944520371413360792609899638051599019982287871594219558531714371280898904236292342272302939591498948592068613570944073257051337951375779402040917686984589621708151504619884114936589082283039714009809118807124745633985765777481561679175826750569315065723796548542381455083496262169171064199")));
	}
	{
		integer num(39);
		integer power(big::pow(num, 67));
		ASSERT_EQ(integer(big::pow(-1, power)), -1);
		ASSERT_EQ(integer(big::pow(1, power)), 1);
	}
}

TEST(AlgorithmTestSuite, TestPowerPolynomial)
{
	using namespace big;

	{
		polynomial poly({1, 1, 1, 1, 1, 1, 1, 1, 1});
		poly = big::pow(poly, 0);

		ASSERT_EQ(poly.to_str(), "1");
	}
	{
		polynomial poly({1, 1, 1, 1, 1, 1, 1, 1, 1});
		poly = big::pow(poly, -1);

		ASSERT_EQ(poly.to_str(), "0");
	}
	{
		polynomial poly({1, 1, 1, 1});
		poly = big::pow(poly, 3);

		ASSERT_EQ(poly.to_str(), "x^9+3*x^8+6*x^7+10*x^6+12*x^5+12*x^4+10*x^3+6*x^2+3*x+1");
	}
	{
		polynomial poly({rational(17, 6), 0, 0, 0, 0, 0, 0, 1});
		poly = big::pow(poly, 4);

		ASSERT_EQ(poly.to_str(), "83521/1296*x^28+4913/54*x^21+289/6*x^14+34/3*x^7+1");
	}
	{
		polynomial happy_ticket({1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
		happy_ticket = big::pow(happy_ticket, 6);

		ASSERT_EQ(happy_ticket[27], 55252);
	}
}

TEST(AlgorithmTestSuite, TestPowerBigNum)
{
	using namespace big;

	auto start = std::chrono::high_resolution_clock::now();
	natural num = big::pow(natural(7), 7);
	num = big::pow(natural(7), num);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::cout << elapsed_seconds.count() << std::endl;
	//std::cout << num << std::endl;
}
