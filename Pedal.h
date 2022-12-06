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
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;
	GLuint VAO, VBOvertex, VBOnormal, EBO;
	GLfloat pointSize;
	glm::vec3 center;
	bool shadeForLight;
	std::vector<Plain*> plains;


public:
	Pedal(std::string objFilename, GLfloat pointSize);
	~Pedal();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float angle, glm::vec3 axis);
	void objParser(string objFilename);
	void scale(float level);
	void translation(glm::vec3 destination);
	glm::vec3 getCenter();
	void setCenter(glm::vec3 center);
	std::vector<Plain*> getPlains() { return this->plains; }

};

#endif
