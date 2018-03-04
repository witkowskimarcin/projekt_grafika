#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
public:
	float x, y, z;
	Vertex(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
#endif // !VERTEX_H