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
	float norm() const;
	float normSquared() const;
	
	// Overloaded Operators
	
	// Identity
	bool operator==(Vector2f v) const;
	
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
};

// Other Overloaded Operators
Vector2f operator*(float s, Vector2f);

//TODO: Vector3f


