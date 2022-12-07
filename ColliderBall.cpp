#include "ColliderBall.h"
////////////////////////////////////////////////////////////////////////////////
ColliderBall::ColliderBall(glm::vec3 origin, GLfloat radius)
{
	this->origin = origin;
	this->radius = radius;
	this->elastic = DEFAULT_BALL_ELASTIC;
	this->represent = new Sphere(this->radius, glm::vec3(1.0,0.95,1));
	this->collider = new Collider(origin, radius,this->elastic);
	this->collider->setOneTime(true);
}

////////////////////////////////////////////////////////////////////////////////

ColliderBall::~ColliderBall()
{
	delete represent;
	delete collider;
}
////////////////////////////////////////////////////////////////////////////////
void ColliderBall::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	renderUpdate();
	represent->draw(viewProjMtx, shader);
}
////////////////////////////////////////////////////////////////////////////////
void ColliderBall::update(GLfloat deltaTime)
{
	if (this->collider->GetHit()) {
		this->represent->changeColor(glm::vec3(0,0,1));
	}
}
