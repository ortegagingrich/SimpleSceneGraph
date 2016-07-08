/**
 * Tests of operations involving lines
 */
#include <cstdio>
#include <gtest/gtest.h>

#include "../src/jvisu/geometry.h"
#include "../src/jvisu/vectormath.h"


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
 * Point-Line Intersection
 */

TEST(TestGeometry, IntersectPointLine){
	/**
	 * TODO: write
	 */
	
	FAIL();
}


/*
 * Line-Line Intersection
 */

TEST(TestGeometry, IntersectLineLine){
	/**
	 * TODO: write
	 */
	
	FAIL();
}



/*
 * Point-Rectangle Intersection
 */

TEST(TestGeometry, IntersectPointRect){
	/**
	 * TODO: write tests
	 */
	
	FAIL();
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

