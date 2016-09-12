/*
 * Declarations and Definitions of some useful data structures and functions
 * for vector, matrix and quaternion operations.
 */
#ifndef VECTORMATH_H
#define VECTORMATH_H

#include "shared_exports.h"

namespace ssg {


	/*
	 * Constants
	 */
	#ifndef M_PI
	#define M_PI 3.14159265358979323846
	#endif

	const double DEG_2_RAD = M_PI / 180.0;
	const double RAD_2_DEG = 180.0 / M_PI;


	/*
	 * Data Structures
	 */
	class SHARED_EXPORT Vector2f {
	public:
		float x, y;
	
		Vector2f();
		Vector2f(float x, float y);
	
		void setZero();
		void set(float xin, float yin);
		void add(float dx, float dy);
		void rotate(float rad);
		void rotateDegrees(float deg){ rotate(deg * DEG_2_RAD); };
		void scale(float s);
		void scale(float sx, float sy);
		void scale(Vector2f sv);
	
		float norm() const;
		float normSquared() const;
		void normalize();
	
		float dot(Vector2f v) const;
		float cross(Vector2f v) const;
	
		// Overloaded Operators
	
		// Literal Assignment
		void operator=(float array[2]);
	
		// Identity
		bool operator==(Vector2f v) const;
		bool operator!=(Vector2f v) const;
	
		// Negation
		Vector2f operator-() const;
	
		// Vector Addition
		void operator+=(Vector2f v);
		void operator-=(Vector2f v);
		Vector2f operator+(Vector2f v) const;
		Vector2f operator-(Vector2f v) const;
	
		// Scalar Multiplication
		void operator*=(float s);
		void operator/=(float s);
		Vector2f operator*(float s) const;
		Vector2f operator/(float s) const;
	
		// Rotation
		void operator%=(float rad);
		Vector2f operator%(float rad) const;
	
	};
	SHARED_EXPORT Vector2f operator*(float s, Vector2f);



	//TODO: Vector3f



}

#endif


