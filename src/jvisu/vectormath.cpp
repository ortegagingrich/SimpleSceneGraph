#include <cmath>
#include "vectormath.h"


/*
 * Vector2f
 */
Vector2f::Vector2f(): x(0), y(0) {}
Vector2f::Vector2f(float xi, float yi): x(xi), y(yi) {}

void Vector2f::setZero(){
	x = 0;
	y = 0;
}

float Vector2f::norm(){
	/**
	 * Calculates and returns the Euclidean norm of the vector.
	 */
	return std::sqrt(normSquared());
}

float Vector2f::normSquared(){
	/**
	 * Calculates and returns the square of the Euclidian norm.
	 */
	return x*x + y*y;
}

