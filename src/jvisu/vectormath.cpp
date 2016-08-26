#include <cstdio>
#include <cmath>
#include "vectormath.h"

using namespace jvisu;


/*
 * Vector2f
 */
Vector2f::Vector2f(): x(0), y(0) {}
Vector2f::Vector2f(float xi, float yi): x(xi), y(yi) {}

void Vector2f::setZero(){
	x = 0;
	y = 0;
}

void Vector2f::set(float xin, float yin){
	x = xin;
	y = yin;
}

void Vector2f::add(float dx, float dy){
	x += dx;
	y += dy;
}

void Vector2f::rotate(float rad){
	rad = fmod(rad, 2*M_PI);
	float cos = std::cos(rad);
	float sin = std::sin(rad);
	float newx = x * cos - y * sin;
	float newy = x * sin + y * cos;
	x = newx;
	y = newy;
}

void Vector2f::scale(float s){
	x *= s;
	y *= s;
}

void Vector2f::scale(float sx, float sy){
	x *= sx;
	y *= sy;
}

void Vector2f::scale(Vector2f sv){
	scale(sv.x, sv.y);
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

void Vector2f::normalize() {
	/**
	 * Normalizes the vector.
	 */
	scale(1.0f / norm());
}

float Vector2f::dot(Vector2f v) const {
	return x * v.x + y * v.y;
}

float Vector2f::cross(Vector2f v) const {
	return x * v.y - y * v.x;
}


/*
 * Overloaded Operators
 */

// Literal Assignment
void Vector2f::operator=(float literal[2]){
	x = literal[0];
	y = literal[1];
}

// Vector Identity

bool Vector2f::operator==(Vector2f v) const {
	return (x == v.x) && (y == v.y);
}

bool Vector2f::operator!=(Vector2f v) const {
	return (x != v.x) || (y != v.y);
}

// Negation

Vector2f Vector2f::operator-() const {
	Vector2f w(-x, -y);
	return w;
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

// Rotation

void Vector2f::operator%=(float rad){
	rotate(rad);
}

Vector2f Vector2f::operator%(float rad) const {
	Vector2f w = *this;
	w.rotate(rad);
	return w;
}




