#include "infrastructure/infrastructure.h"

#include "SimpleMath.h"

TEST(SimpleAdd, TestSimpleAdd_0and1_Expects0) {
	int result = simple_add(0, 1);

	EXPECT_EQ(1, result);
}

TEST(SimpleAdd, TestSimpleAdd_4and2_Expects3) {
	int result = simple_add(4, 2);

	EXPECT_EQ(6, result);
}

TEST(SimpleSub, TestSimpleSub_0and1_Expects0) {
	int result = simple_sub(0, 1);

	EXPECT_EQ(-1, result);
}

TEST(SimpleSub, TestSimpleSub_4and2_ExpectsNeg1) {
	int result = simple_sub(4, 2);

	EXPECT_EQ(2, result);
}

TEST(SimpleMul, TestSimpleMul_0and1_Expects0) {
	int result = simple_mul(0, 1);

	EXPECT_EQ(0, result);
}

TEST(SimpleMul, TestSimpleMul_4and2_Expects2) {
	int result = simple_mul(4, 2);

	EXPECT_EQ(8, result);
}

TEST(SimpleDiv, TestSimpleDiv_0and1_Expects0) {
	int result = simple_div(0, 1);

	EXPECT_EQ(0, result);
}

TEST(SimpleDiv, TestSimpleDiv_4and2_Expects2) {
	int result = simple_div(4, 2);

	EXPECT_EQ(2, result);
}
