/**
 * Tests of operations involving lines
 */
#include <cstdio>
#include <gtest/gtest.h>

#include "../src/ssg/ssg.h"

#include "../src/ssg/geometry.h"
#include "../src/ssg/vectormath.h"


using namespace ssg;


static void printLine(Line2f line){
	/*
	 * Just a quick function for debugging purposes.
	 */
	printf("\n\n");
	printf("Line from (%f, %f) ", line.startPoint.x, line.startPoint.y);
	printf("to (%f, %f)\n", line.endPoint.x, line.endPoint.y);
	printf("\n\n");
}



/*
 * Line-Line Intersection
 */

TEST(TestGeometry, IntersectLineLine){
	Line2f l1, l2;
	Vector2f out;
	bool result;
	
	// Trial 1: Parallel
	out.set(3.14159, 2.71828);
	l1.set(-1, 0, -1, 42);
	l2.set(47, 0, 47, 42);
	result = calculate_intersection(l1, l2, out);
	
	EXPECT_FALSE(result);
	EXPECT_EQ(out, Vector2f(3.14159, 2.71828));
	
	
	// Trial 2: Collinear
	out.set(3.14159, 2.71828);
	l1.set(-1, 3.14159, -1, 42);
	l2.set(-1, 0, -1, 47);
	result = calculate_intersection(l1, l2, out);
	
	EXPECT_FALSE(result);
	EXPECT_EQ(out, Vector2f(3.14159, 2.71828));
	
	
	// Trial 3: Regular Intersection
	l1.set(-1, -2, 2, 4);
	l2.set(3, 0, 0, 3);
	result = calculate_intersection(l1, l2, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Vector2f(1, 2));
	
	
	// Trial 4: T intersection
	l1.set(-47, -42, 0, 0);
	l2.set(-51, -43, -43, -41);
	result = calculate_intersection(l1, l2, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Vector2f(-47, -42));
	
	
	// Trial 5: Intersection Outside of Range
	l1.set(11, 8, 12, 9);
	l2.set(9, 9, 8, 11);
	result = calculate_intersection(l1, l2, out);
	
	EXPECT_FALSE(result);
	EXPECT_EQ(out, Vector2f(10, 7));
	
}



/*
 * Point-Rectangle Intersection
 */

TEST(TestGeometry, IntersectPointRect){
	Rect2f rect(3.14159, 4, -47, 42);
	Vector2f point;
	bool result;
	
	// Trial 1: Inside
	point.set(3.5, 1.2);
	result = calculate_intersection(rect, point);
	
	EXPECT_TRUE(result);
	
	
	// Trial 2: Outside Horizontally
	point.set(0, 1.2);
	result = calculate_intersection(rect, point);
	
	EXPECT_FALSE(result);
	
	
	// Trial 3: Outside Vertically
	point.set(3.5, 47);
	result = calculate_intersection(rect, point);
	
	EXPECT_FALSE(result);
	
	
	// Trial 4: Completely Outside
	point.set(-666, -555);
	result = calculate_intersection(rect, point);
	
	EXPECT_FALSE(result);
	
	
	// Trial 5: Edge
	point.set(3.14159, 2.71828);
	result = calculate_intersection(rect, point);
	
	EXPECT_TRUE(result);
	
	
	// Trial 6: Corner
	point.set(3.14159, 42);
	result = calculate_intersection(rect, point);
	
	EXPECT_TRUE(result);
	
}


/*
 * Line-Rectangle Intersection
 */

TEST(TestGeometry, IntersectLineRect){
	Rect2f rect(-1.0f, 1.0f, -0.5f, 0.5f);
	Line2f line, out;
	bool result;
	
	// Trial 1: Completely Within
	line.set(-0.7f, 0.2f, 0.1f, -0.4f);
	result = calculate_intersection(rect, line, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(line, out);
	
	
	// Trial 2: Completely Without
	line.set(-47, 3.14f, 42, 2.7f);
	result = calculate_intersection(rect, line);
	
	EXPECT_FALSE(result);
	
	
	// Trial 3: One in, one out (side)
	line.set(-1.5f, 0.f, 0.f, 0.f);
	result = calculate_intersection(rect, line, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Line2f(-1.0f, 0.f, 0.f, 0.f));
	
	
	// Trial 4: One in, one out (top)
	line.set(0.f, 0.f, 1.2f, 120);
	result = calculate_intersection(rect, line, out);
	
	EXPECT_TRUE(result);
	// Need different comparison because of floating point precision
	EXPECT_TRUE((out.startPoint - Vector2f(0, 0)).norm() < 1e-6);
	EXPECT_TRUE((out.endPoint - Vector2f(0.005f, 0.5f)).norm() < 1e-6);
	
	
	// Trial 5: Across
	line.set(-2.f, 0.231f, 2.f, 0.231f);
	result = calculate_intersection(rect, line, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Line2f(1.f, 0.231, -1.f, 0.231));
	
	
	// Trial 6: Corner Across
	line.set(0.f, 0.75f, 1.5f, 0.f);
	result = calculate_intersection(rect, line, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Line2f(0.5f, 0.5f, 1.f, 0.25f));
	
}


/*
 * Rectangle-Rectangle intersection
 */

TEST(TestGeometry, IntersectRectRect){
	Rect2f r1, r2, out;
	bool result;
	
	// Trial 1: No Overlap
	r1.set(-2, -1, -1, 1);
	r2.set(1, 2, -1, 1);
	result = calculate_intersection(r1, r2);
	
	EXPECT_FALSE(result);
	
	
	// Trial 2: One Inside
	r1.set(-2, 2, -2, 2);
	r2.set(-1, 1, -1, 1);
	result = calculate_intersection(r1, r2, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, r2);
	
	// Trial 2.5: Reversed
	result = calculate_intersection(r2, r1, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, r2);
	
	
	// Trial 3: Cross Intersection
	r1.set(-2, 2, -1, 1);
	r2.set(-1, 1, -2, 2);
	result = calculate_intersection(r1, r2, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Rect2f(-1, 1, -1, 1));
	
	
	// Trial 4: Corner Intersection
	r1.set(-2, 0, 0, 2);
	r2.set(-1, 1, -1, 1);
	result = calculate_intersection(r1, r2, out);
	
	EXPECT_TRUE(result);
	EXPECT_EQ(out, Rect2f(-1, 0, 0, 1));
	
}

