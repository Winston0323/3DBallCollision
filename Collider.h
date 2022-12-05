#pragma once
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "core.h"
#include "Sphere.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

class Collider
{
private:
	//parameters
	glm::vec3 center;
	GLfloat radius;
	GLfloat elastic;
	int state; //1 is sphere, 2 is plain, 3 is edge
	glm::vec3 norm;
	std::vector<glm::vec3> positions;

public:
	Collider(glm::vec3 center, GLfloat radius, GLfloat elastic);
	Collider(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 norm, GLfloat elastic);
	Collider(glm::vec3 p1, glm::vec3 p2, GLfloat elastic);
	~Collider();

	//getters
	GLfloat GetRadius() { return this->radius; }
	glm::vec3 GetCenter() { return this->center; }
	GLfloat GetElastic() { return this->elastic; }
	glm::vec3 GetNorm(glm::vec3 pos);
	glm::vec3 GetNorm() { return this->norm; }
	int GetState() { return this->state; }
	
	//setters
	void setCenter(glm::vec3 val) { this->center = val; }
	void setRadius(GLfloat val) { this->radius = val; }
	//getter for imgui
	GLfloat* getElastic() { return &this->elastic; }
	std::pair<bool, GLfloat> checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel,GLfloat rad);
	bool checkSide(glm::vec3 pos);
	void translate(glm::vec3 destination) { this->center = destination; }
};

////////////////////////////////////////////////////////////////////////////////

#endif
