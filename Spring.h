#ifndef _SPRING_H_
#define _SPRING_H_

#include "core.h";
////////////////////////////////////////////////////////////////////////////////

class Spring
{
private:
	GLfloat springConst, dampingConst, restLength;
	glm::vec3 pos1, pos2;
	glm::vec3 p1StartPos, p2StartPos;

public:
	Spring(glm::vec3 pos1, glm::vec3 pos2, 
		GLfloat SConst, GLfloat RLength);
	~Spring();
	void ComputeForce();
};

////////////////////////////////////////////////////////////////////////////////

#endif

