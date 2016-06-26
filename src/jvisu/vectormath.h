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
};

//TODO: Vector3f


