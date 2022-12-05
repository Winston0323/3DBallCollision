#include "BounceBall.h"
////////////////////////////////////////////////////////////////////////////////
BounceBall::BounceBall(glm::vec3 origin, GLfloat radius)
{
	this->origin = origin;
	this->radius = radius;
	this->elastic = DEFAULT_BALL_ELASTIC;
	this->represent = new Sphere(this->radius, glm::vec3(1.0,0.95,1));
	this->collider = new Collider(origin, radius,this->elastic);
}

////////////////////////////////////////////////////////////////////////////////

BounceBall::~BounceBall()
{
	delete represent;
	delete collider;
}
////////////////////////////////////////////////////////////////////////////////
void BounceBall::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	represent->draw(viewProjMtx, shader);
	//ps->draw(view, projection, pShader);
}
////////////////////////////////////////////////////////////////////////////////

