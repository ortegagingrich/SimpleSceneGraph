/*
 * Unit Tests of vectormath.h/vectormath.cpp
 */
#include <gtest/gtest.h>
#include "../src/jvisu/vectormath.h"


TEST(TestVectormath, Vector2fIdentity){
	Vector2f v1(42.0, -47);
	Vector2f v2 = v1;
	EXPECT_EQ(v1, v2);
}


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


TEST(TestVectormath, Vector2fSubtraction){
	Vector2f v1(1, -4);
	Vector2f v2(3.5f,  2);
	Vector2f v3(-99.1f, 12.32f);
	
	Vector2f v4 = v2 - v3;
	Vector2f v5 = v1;
	v5 -= v2;
	
	EXPECT_EQ(v2.x - v3.x, v4.x);
	EXPECT_EQ(v2.y - v3.y, v4.y);
	EXPECT_EQ(v1.x - v2.x, v5.x);
	EXPECT_EQ(v1.y - v2.y, v5.y);
}


TEST(TestVectormath, Vector2fScalarMultiplication){
	Vector2f v1(1, -4);
	Vector2f v2 = 0.5f * v1;
	Vector2f v3 = v2 * 0.5f;
	
	EXPECT_EQ(v1/4, v3);
	
	v1 /= 2.0;
	v3 *= 2;
	
	EXPECT_EQ(v1, v2);
	EXPECT_EQ(v1, v3);
	
}


TEST(TestVectormath, Vector2fNorms){
	Vector2f vectors[4] = {
		Vector2f(3.0, 4.0),
		Vector2f(-3.0, 4.0),
		Vector2f(4.0, 3.0),
		Vector2f(-4.0, -3.0)
	};
	
	for(int i = 0; i < 4; i++){
		Vector2f vector = vectors[i];
		EXPECT_EQ(vector.norm(), 5.0);
		EXPECT_EQ(vector.normSquared(), 25.0);
	}
}




































