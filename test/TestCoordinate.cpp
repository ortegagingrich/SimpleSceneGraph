/*
 * Some unit tests for coordinate transformations
 */
#include <cstdio>
#include <gtest/gtest.h>

#include "../src/jvisu/jvisu.h"

#include "../src/jvisu/viewport.h"


using namespace jvisu;


TEST(CoordinateTransform2D, viewportToWorld){
	Viewport2D viewport;
	
	float xin, xout, yin, yout;
	
	/*
	 * Baseline tests
	 */
	xin = 0.0f;
	yin = 0.0f;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, 0.0f);
	EXPECT_EQ(yout, 0.0f);
	
	xin = 1.542;
	yin = -35;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin);
	EXPECT_EQ(yout, yin);
	
	/*
	 * Off-Center; No scaling
	 */
	viewport.setCenter(1.0f, -3.5f);
	
	xin = 0.0f;
	yin = 0.0f;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, 1.0f);
	EXPECT_EQ(yout, -3.5f);
	
	xin = 1.542;
	yin = -35;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin + 1.0f);
	EXPECT_EQ(yout, yin - 3.5f);
	
	/*
	 * Centered, aspect ratio
	 */
	viewport.setCenter(0, 0);
	viewport.setRadiusX(2.0f);
	
	xin = 0.0f;
	yin = 0.0f;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, 0.0f);
	EXPECT_EQ(yout, 0.0f);
	
	xin = 0.75;
	yin = -0.35;
	viewport.viewportToWorld(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin * 2);
	EXPECT_EQ(yout, yin * 2);
}


TEST(CoordinateTransform2D, worldToViewport){
	Viewport2D viewport;
	
	float xin, xout, yin, yout;
	
	/*
	 * Baseline tests
	 */
	xin = 0.0f;
	yin = 0.0f;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, 0.0f);
	EXPECT_EQ(yout, 0.0f);
	
	xin = 1.542;
	yin = -35;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin);
	EXPECT_EQ(yout, yin);
	
	/*
	 * Off-Center; No scaling
	 */
	viewport.setCenter(1.0f, -3.5f);
	
	xin = 0.0f;
	yin = 0.0f;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, -1.0f);
	EXPECT_EQ(yout, 3.5f);
	
	xin = 1.542;
	yin = -3;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin - 1.0f);
	EXPECT_EQ(yout, yin + 3.5f);
	
	/*
	 * Centered, aspect ratio
	 */
	viewport.setCenter(0, 0);
	viewport.setRadiusX(2.0f);
	
	xin = 0.0f;
	yin = 0.0f;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, 0.0f);
	EXPECT_EQ(yout, 0.0f);
	
	xin = 0.75;
	yin = -0.35;
	viewport.worldToViewport(xin, yin, xout, yout);
	EXPECT_EQ(xout, xin * 0.5f);
	EXPECT_EQ(yout, yin * 0.5f);
}




