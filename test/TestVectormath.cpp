/*
 * Unit Tests of vectormath.h/vectormath.cpp
 */
#include <gtest/gtest.h>
#include "../src/jvisu/vectormath.h"


TEST(TestVectormath, Vector2fAddition){
	Vector2f v1(1, -4);
	Vector2f v2(3.5f,  2);
	Vector2f v3(-99.1f, 12.32f);
	
	Vector2f v4 = v2 + v3;
	Vector2f v5 = v1;
	v5 += v2;
	
	EXPECT_EQ(v2.x + v3.x, v4.x);
	EXPECT_EQ(v2.y + v3.y, v4.y);
	EXPECT_EQ(v1.x + v2.x, v5.x);
	EXPECT_EQ(v1.y + v2.y, v5.y);
}

