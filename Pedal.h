#ifndef _PEDAL_H_
#define _PEDAL_H_

#include "Object.h"
#include "Plain.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class Pedal : public Object
{
private:

	//render datas
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> localPos;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	std::vector<std::pair<int, int>> edges;
	std::vector<std::pair<glm::vec3, glm::vec3>> obtedges;

	//renders	
	GLuint VAO, VBOvertex, VBOnormal, EBO;

	//centers
	glm::vec3 massCenter;
	glm::vec3 rightMassCenter;
	glm::mat3 rotMat;
	glm::quat rotQuat;
	int state = 0;//0 is resting, 1 is moving up, 2 is holding, 3 is moving down
	bool flip;

	//collision
	std::vector<Collider*> colliders;
	GLfloat elastic;
	
	//animation
	GLfloat moveDegree = 0.0f;
	GLfloat moveLimit = 60.0f;
	GLfloat moveDuration = 0.5f;

	//3 dimension
	GLfloat xMax;
	GLfloat xMin;
	GLfloat yMax;
	GLfloat yMin;
	GLfloat zMax;
	GLfloat zMin;


public:
	Pedal(glm::vec3 initPos, GLfloat initSpin,bool flip);
	~Pedal();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update(GLfloat deltaTime);
	void update();

	void spin(float angle, glm::vec3 axis);
	void spinPerVetex(float angle, glm::vec3 axis);
	void objParser(string objFilename);
	void scale(float xlevel, float ylevel, float zlevel);
	void translation(glm::vec3 destination);
	void translationPerVertex(glm::vec3 destination);
	void moveToWorldCenter();

	//getter
	glm::vec3 GetMassCenter() { return this->massCenter; }

	GLfloat	GetXMax() { return this->xMax; }
	GLfloat	GetXMin() { return this->xMin; }
	GLfloat	GetYMax() { return this->yMax; }
	GLfloat	GetYMin() { return this->yMin; }
	GLfloat	GetZMax() { return this->zMax; }
	GLfloat	GetZMin() { return this->zMin; }
	
	GLfloat	GetAngVel();

	std::vector<glm::vec3> localToWorld(glm::vec3 massCenter, glm::mat3 rotMat);
	void setCollider(std::vector<Collider*> val) { this->colliders = val; }
	void setRotationMatrix(GLfloat degree, glm::vec3 axis) {
		this->rotMat = glm::mat3(glm::rotate(glm::radians(degree), axis)) * this->rotMat;
	}
	void setState(int val) { this->state = val; }

	std::vector<Collider*> GetCollider() { return this->colliders; }
	void moveStick(GLfloat deltaTime);

	void restoreDefault();
	void updateThreeDegree();

};

#endif
