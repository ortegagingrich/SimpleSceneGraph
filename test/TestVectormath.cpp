/*
 * Unit Tests of vectormath.h/vectormath.cpp
 */
#include <cstdio>
#include <gtest/gtest.h>

#include "../src/ssg/ssg.h"

#include "../src/ssg/vectormath.h"


using namespace ssg;



const double TOLERANCE = 1e-6;
const double TOLERANCE_LENIENT = 1e-3;


TEST(TestVectormath, Vector2fIdentity){
	Vector2f v1(42.0, -47);
	Vector2f v2 = v1;
	EXPECT_EQ(v1, v2);
}


TEST(TestVectormath, Vector2fNegation){
	Vector2f v1(42.0, -47);
	Vector2f v2 = -v1;
	EXPECT_EQ(v2, Vector2f(-42.0, 47));
	EXPECT_EQ(v1, -v2);
	EXPECT_EQ(-v1, v2);
}


TEST(TestVectormath, Vector2fLiteralAssignment){
	Vector2f v1;
	v1 = {42.0, -47};
	Vector2f v2(42.0, -47);
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


TEST(TestVectormath, Vector2fRotation){
	Vector2f right(1, 0);
	Vector2f up(0, 1);
	
	Vector2f end1 = right % (M_PI/2);
	Vector2f end2 = right;
	Vector2f end3 = right;
	end2.rotateDegrees(90);
	end3.rotate(-7*M_PI/2);
	
	
	EXPECT_LE((end1 - up).norm(), TOLERANCE);
	EXPECT_LE((end2 - up).norm(), TOLERANCE);
	EXPECT_LE((end3 - up).norm(), TOLERANCE);
	
	// Full rotations
	int rotations[] = {1, 2, 99, 1000};
	int increments[] = {1000, 500, 99, 2};
	for(int j = 0; j < 4; j++){
		Vector2f dizzy(1, 0);
		
		int rot = rotations[j];
		int inc = increments[j];
		printf("Rotation test: %d rotations with %d increments per rotation\n", rot, inc);
		
		float rotation_angle = 2*M_PI / inc;
		
		for(int r = 0; r < rot; r++){
			for(int i = 0; i < inc; i++){
				dizzy %= rotation_angle;
			}
		}
		
		EXPECT_LE((dizzy - right).norm(), TOLERANCE_LENIENT);
	}
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


TEST(TestVectormath, Vector2fNormalize){
	Vector2f v1, v2, v3, v4, v5, zero;
	v1 = {3.0, 4.0};
	v2 = {0.6, 0.8};
	v3 = {-138.515681f, 9999999999};
	v4 = {1e-8, -5.46e-9};
	v5 = v2;
	zero = {0, 0};
	
	v1.normalize();
	v3.normalize();
	v4.normalize();
	v5.normalize();
	zero.normalize();
	
	EXPECT_EQ(v1, v2);
	EXPECT_EQ(v5, v2);
	EXPECT_EQ(v3.norm(), 1);
	EXPECT_EQ(v4.norm(), 1);
	EXPECT_TRUE(zero.x != zero.x);
	EXPECT_TRUE(zero.y != zero.y);
}


