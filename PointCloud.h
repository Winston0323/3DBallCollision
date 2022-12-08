#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"
#include "Plain.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

class PointCloud : public Object
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
	glm::mat4 smodel = glm::mat4(1);
	glm::mat4 rmodel = glm::mat4(1);
	glm::mat4 tmodel = glm::mat4(1);


public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float angle, glm::vec3 axis);
	void objParser(string objFilename);
	void scale(float level);
	void clearScale() { this->smodel = glm::mat4(1); }
	void translation(glm::vec3 destination);
	glm::vec3 getCenter();
	void setCenter(glm::vec3 center);
	void setColor(glm::vec3 val) { this->color = val; }
	std::vector<Plain*> getPlains() { return this->plains; }

};

#endif