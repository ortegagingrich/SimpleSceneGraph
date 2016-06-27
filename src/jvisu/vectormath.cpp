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

float Vector2f::norm() const {
	/**
	 * Calculates and returns the Euclidean norm of the vector.
	 */
	return std::sqrt(normSquared());
}

float Vector2f::normSquared() const {
	/**
	 * Calculates and returns the square of the Euclidian norm.
	 */
	return x*x + y*y;
}


/*
 * Overloaded Operators
 */

// Vector Identity

bool Vector2f::operator==(Vector2f v) const {
	return (x == v.x) && (y == v.y);
}

// Vector Addition

void Vector2f::operator+=(Vector2f v){
	x += v.x;
	y += v.y;
}

void Vector2f::operator-=(Vector2f v){
	x -= v.x;
	y -= v.y;
}

Vector2f Vector2f::operator+(Vector2f v) const {
	Vector2f w(x + v.x, y + v.y);
	return w;
}

Vector2f Vector2f::operator-(Vector2f v) const {
	Vector2f w(x - v.x, y - v.y);
	return w;
}

// Scalar Multiplication

void Vector2f::operator*=(float scalar){
	x *= scalar;
	y *= scalar;
}

void Vector2f::operator/=(float scalar){
	x /= scalar;
	y /= scalar;
}

Vector2f Vector2f::operator*(float scalar) const {
	Vector2f w = *this;
	w *= scalar;
	return w;
}

Vector2f Vector2f::operator/(float scalar) const {
	Vector2f w = *this;
	w /= scalar;
	return w;
}

Vector2f operator*(float scalar, Vector2f v){
	return v * scalar;
}








































