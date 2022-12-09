#pragma once
#ifndef _BALL_H_
#define _BALL_H_

#include "core.h"
#include "Sphere.h"
#include "Plain.h"
#include <iostream>
#include "Collider.h"

////////////////////////////////////////////////////////////////////////////////

class Ball
{
private:
	//parameters
	glm::vec3 position, velocity, force,
		gravForce, gravForceMult, norm, windVelocity;
	GLfloat mass, radius, pointSize, elastic,
		friction, airResist, gravMult, grav;

	Sphere* sphere;

	std::vector<Collider*> colliders;

public:
	Ball();
	~Ball();

	void update(GLfloat timeStep, GLfloat& restTime);
	void draw(const glm::mat4& viewProjMtx, GLuint shader, GLuint pShader);
	void ApplyForce(glm::vec3 f);

	glm::vec3 GetPos();
	glm::vec3 GetVelocity();
	GLfloat GetRadius() { return this->radius; }

	void addNorm(glm::vec3 normal);
	void clearNorm();

	void normalize();
	void translate(float dist);

	glm::vec3 getNorm();
	glm::vec3 getPos();
	void boxCollision(GLfloat timeStep, GLfloat& restTime);
	void resting();
	void renderUpdate();
	void changeColor(glm::vec3 color) { this->sphere->changeColor(color); }
	std::vector<glm::vec3> intRK( GLfloat timeStep);

	void defaultForces();
	GLfloat* getMass();
	GLfloat* getGravMult();
	void setGravMult(GLfloat val) { this->gravMult = val; }
	void setColor(glm::vec3 val) { this->sphere->changeColor(val); }
	void setVelocityX(GLfloat val) { 
		this->velocity = glm::vec3(-val, 0, 0); }
	GLfloat* getAirResist();
	GLfloat getRadius();
	GLfloat* getSphereRadius() { this->radius = *sphere->getRadius(); return sphere->getRadius(); }
	GLfloat* getFriction();
	GLfloat* getElastic();
	GLfloat* getPosX();
	GLfloat* getPosY();
	GLfloat* getPosZ();
	GLfloat* getWindX() { return &this->windVelocity.x; }
	GLfloat* getWindY() { return &this->windVelocity.y; }
	GLfloat* getWindZ() { return &this->windVelocity.z; }
	GLfloat* getVelX();
	GLfloat* getVelY();
	GLfloat* getVelZ();
	//setter
	void setCollider(std::vector<Collider*> val) { this->colliders = val; }
	void restoreDefault();
};

////////////////////////////////////////////////////////////////////////////////

#endif
