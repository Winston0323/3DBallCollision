#ifndef _BOUNCEBALL_H_
#define _BOUNCEBALL_H_

#include "core.h"
#include "Plain.h"
#include "Collider.h"
#include "PointCloud.h"

////////////////////////////////////////////////////////////////////////////////

class BounceBall
{
private:
	bool toggle;
	GLfloat elastic;
	GLfloat radius;
	glm::vec3 origin;
	Sphere* represent;
	Collider* collider;
	std::string fileName = "obj/sphere.obj";


public:
	BounceBall(glm::vec3 origin, GLfloat radius );
	~BounceBall();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void translate(glm::vec3 destination) {
		this->represent->translation(destination);
		this->collider->translate(destination);
	}
	void update(GLfloat deltaTime);
	//getters
	glm::vec3 GetOrigin()	{ return origin; }
	bool GetToggle()	{ return this->toggle; }
	GLfloat GetElastic()	{ return this->elastic; }
	GLfloat GetRadius()		{ return this->radius; }
	Collider* GetCollider() { return this->collider; }

	//getter for imgui
	GLfloat* getOriginX()	{ return &this->origin.x; }
	GLfloat* getOriginY()	{ return &this->origin.y; }
	GLfloat* getElastic()	{ return &this->elastic; }
	GLfloat* getRadius()	{ return &this->radius; }
	GLfloat* getSphereRadius() {	this->radius = *represent->getRadius(); 
									this->collider->setRadius(radius);
									return represent->getRadius(); }

	void renderUpdate() { this->translate(this->origin); }

};

////////////////////////////////////////////////////////////////////////////////

#endif
