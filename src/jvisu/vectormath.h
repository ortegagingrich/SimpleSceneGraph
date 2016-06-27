/*
 * Declarations and Definitions of some useful data structures and functions
 * for vector, matrix and quaternion operations.
 */

/*
 * Data Structures
 */
class Vector2f {
public:
	float x, y;
	
	Vector2f();
	Vector2f(float x, float y);
	
	void setZero();
	float norm();
	float normSquared();
	
	// Overloaded Operators
	void operator+=(Vector2f v);
	void operator-=(Vector2f v);
	Vector2f operator+(Vector2f v);
	Vector2f operator-(Vector2f v);
};

//TODO: Vector3f


