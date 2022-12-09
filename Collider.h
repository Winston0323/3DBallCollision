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
	GLuint VAO, VBO;
	glm::vec3 center;
	glm::vec3 peak;
	GLfloat radius;
	GLfloat elastic;
	bool hit = false;
	bool toggle = true;
	bool oneTime = false;

	int state; //1 is sphere, 2 is plain, 3 is edge, 4 is one way wall, 5 is no response collider, 6 is portal
	glm::vec3 norm;
	std::vector<glm::vec3> positions;

public:
	Collider(glm::vec3 center, GLfloat radius, GLfloat elastic);
	Collider(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 norm, GLfloat elastic, bool oneWay);
	Collider(GLfloat elastic, glm::vec3 anker, glm::vec3 peak);
	Collider(glm::vec3 p1, glm::vec3 p2, GLfloat elastic);
	~Collider();
	void draw(const glm::mat4& viewProjMtx, GLuint shader)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// Bind VAO
		glBindVertexArray(VAO);

		// Bind VBO to the bound VAO, and store the point data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
		// Enable Vertex Attribute 0 to pass point data through to the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Unbind the VBO/VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		// Actiavte the shader program 
		glUseProgram(shader);

		glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
		glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(glm::vec3(1,0,0)));

		// Bind the VAO
		glBindVertexArray(VAO);

		// Set point size
		glPointSize(10);

		// Draw the points 
		glDrawArrays(GL_POINTS, 0,	positions.size());

		// Unbind the VAO and shader program
		glBindVertexArray(0);
		glUseProgram(0);
	}

	//getters
	GLfloat GetRadius() { return this->radius; }
	glm::vec3 GetCenter() { return this->center; }
	GLfloat GetElastic() { return this->elastic; }
	glm::vec3 GetNorm(glm::vec3 pos);
	glm::vec3 GetNorm() { return this->norm; }
	bool GetHit() { return this->hit; }
	bool GetToggle() { return this->toggle; }
	glm::vec3 velResp(glm::vec3 pos, GLfloat angularVel);
	int GetState() { return this->state; }
	
	//setters
	void setCenter(glm::vec3 val) { this->center = val; }
	void setRadius(GLfloat val) { this->radius = val; }
	void setState(int val) { this->state = val; }
	void setToggle(bool val) { this->toggle = false; }
	void setElastic(GLfloat val) { this->elastic = val; }
	void setHit(bool val) { this->hit = val; this->toggle = !this->toggle; }
	//getter for imgui
	GLfloat* getElastic() { return &this->elastic; }
	std::pair<bool, GLfloat> checkHit(glm::vec3 pos, glm::vec3 nextPos, glm::vec3 vel,GLfloat rad);
	bool checkSide(glm::vec3 pos);
	void translate(glm::vec3 destination) { this->center = destination; }
	void spin(glm::vec3 referPnt, glm::vec3 peak);
	void setOneTime(bool val) { this->oneTime = val; }
};

////////////////////////////////////////////////////////////////////////////////

#endif
