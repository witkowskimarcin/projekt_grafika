/*
 *	Na podstawie: OpenGL Super Bible!
 */

#include <cmath>

#include "vmath.h"

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length 1.
void ReduceToUnit(vec3f vector)
{
	float length;
	// Calculate the length of the vector
	length = (float)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));
	// Keep the program from blowing up by providing an acceptable
	// value for vectors whose length may be calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;
	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}

// Points p1, p2, & p3 specified in counterclockwise order
void calcNormal(vec3f v[3], vec3f out)
{
	vec3f v1, v2;
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];
	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];
	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out[]
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];
	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}

