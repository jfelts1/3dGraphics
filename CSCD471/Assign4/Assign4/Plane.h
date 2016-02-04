#ifndef PLANE_H
#define PLANE_H


#include <GL/glew.h>


class Plane
{

private:
	unsigned int vao;

public:
	Plane();

	void render();
};

#endif //PLANE_H
