#include "Pedal.h"


Pedal::Pedal(glm::vec3 initPos, GLfloat initSpin,bool flip)
{
	string objFilename ="";
	if (flip) {
		objFilename = "obj/coneflip.obj";
	}
	else {
		objFilename = "obj/cone.obj";
	}
	this->flip = flip;

	objParser(objFilename);//setup render data
	spinPerVetex(initSpin,glm::vec3(0,0,1));
	restoreDefault();//setup constance
	moveToWorldCenter();//correct render data
	translationPerVertex(initPos);
	//generate collider 
	//glm::vec3 pos1 = glm::vec3(this->massCenter.x, this->massCenter.y, this->massCenter.z - 2);
	//glm::vec3 pos2 = glm::vec3(this->massCenter.x, this->massCenter.y, this->massCenter.z + 2);
	//glm::vec3 pos3 = this->points[32];
	//glm::vec3 norm = glm::normalize(glm::cross(pos2 - pos1, pos3 - pos1));
	this->colliders.push_back(new Collider(0.8, this->massCenter, this->points[32]));
	this->colliders.push_back(new Collider(0.8, this->rightMassCenter, this->points[32]));
	//rendering set up
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOvertex);
	glGenBuffers(1, &VBOnormal);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//generate ebo 
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);


	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Pedal::~Pedal()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBOvertex);
	glDeleteBuffers(1, &VBOnormal);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void Pedal::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	//this->colliders[1]->draw(viewProjMtx, shader);
	// actiavte the shader program 
	glDisable(GL_CULL_FACE);
	glUseProgram(shader);
	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
	glUniform4fv(glGetUniformLocation(shader, "astColor"), 1, glm::value_ptr(this->color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBOvertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);


	// Bind VBO to the bound VAO, and store the normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOnormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);

	//// Set point size
	//glPointSize(1);
	//// Draw the points 
	//glDrawArrays(GL_POINTS, 0, points.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Pedal::update(GLfloat deltaTime)
{
	this->points = localToWorld(this->massCenter, this->rotMat);
	this->colliders[0]->spin(this->massCenter, this->points[32]);
	this->colliders[1]->spin(this->massCenter, this->points[32]);
	if (state == 1) {
		//std::cout << "Moving Up" << std::endl;
		moveStick(deltaTime);
	}else if (state == 3) {
		//std::cout << "Moving down" << std::endl;
		moveStick(-deltaTime);
	}
	
}

void Pedal::update()
{
	this->points = localToWorld(this->massCenter, this->rotMat);
}

void Pedal::spin(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::radians(angle), axis) * model;

}
void Pedal::spinPerVetex(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	for (int i = 0; i < points.size(); i++) {
		this->points[i] = glm::vec3(glm::rotate(glm::radians(angle), axis) * glm::vec4(this->points[i], 1.0f));
	}
}
void Pedal::scale(float xlevel, float ylevel, float zlevel)
{
	for (int i = 0; i < points.size(); i++) {
		this->points[i].x = this->points[i].x * xlevel;
		this->points[i].y = this->points[i].y * ylevel;
		this->points[i].z = this->points[i].z * zlevel;
	}
}
void Pedal::translation(glm::vec3 destination)
{
	//move the masss center to the destination point
	this->massCenter = glm::vec3(destination);
	this->rightMassCenter = this->massCenter + rotMat * this->localPos[12];
	model = glm::translate(glm::mat4(1.0f), destination);
}
void Pedal::translationPerVertex(glm::vec3 destination)
{
	this->massCenter = glm::vec3(destination);
	this->rightMassCenter = this->massCenter + rotMat * this->localPos[12];
}

void Pedal::objParser(string objFilename)
{
	std::ifstream objFile(objFilename); // The obj file we are reading.
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> faces;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}
			if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;

				// Process the point. For example, you can save it to a.
				normals.push_back(normal);
			}
			if (label == "f")
			{
				glm::ivec3 face;
				string x, y, z;
				int xCord, yCord, zCord;
				ss >> x >> y >> z;
				xCord = stoi(x.substr(0, x.find("//"))) - 1;
				yCord = stoi(y.substr(0, y.find("//"))) - 1;
				zCord = stoi(z.substr(0, z.find("//"))) - 1;
				face.x = xCord;
				face.y = yCord;
				face.z = zCord;
				faces.push_back(face);
			}
		}

		//update points normals and faces
		this->points = points;
		this->normals = normals;
		this->faces = faces;
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();
}
void Pedal::moveToWorldCenter() {
	std::vector<GLfloat> xCord;
	std::vector<GLfloat> yCord;
	std::vector<GLfloat> zCord;
	for (int i = 0; i < points.size(); i++) {
		xCord.push_back(this->points[i].x);
		yCord.push_back(this->points[i].y);
		zCord.push_back(this->points[i].z);
	}

	GLfloat midX = (*std::max_element(xCord.begin(), xCord.end()) + *std::min_element(xCord.begin(), xCord.end())) / 2;
	GLfloat midY = (*std::max_element(yCord.begin(), yCord.end()) + *std::min_element(yCord.begin(), yCord.end())) / 2;
	GLfloat midZ = (*std::max_element(zCord.begin(), zCord.end()) + *std::min_element(zCord.begin(), zCord.end())) / 2;
	//initialize

	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - midX;
		points[i].y = points[i].y - midY;
		points[i].z = points[i].z - midZ;
	}
	scale(0.4,1.3,0.3);

	this->massCenter = this->points[28];
	this->rightMassCenter = this->points[12];
	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - this->massCenter.x;
		points[i].y = points[i].y - this->massCenter.y;
		points[i].z = points[i].z - this->massCenter.z;
	}

	std::vector<GLfloat> xCord1;
	std::vector<GLfloat> yCord1;
	std::vector<GLfloat> zCord1;
	for (int i = 0; i < points.size(); i++) {
		xCord1.push_back(this->points[i].x);
		yCord1.push_back(this->points[i].y);
		zCord1.push_back(this->points[i].z);
	}
	//get the three size of the rigid body
	this->xMax = *std::max_element(xCord1.begin(), xCord1.end());
	this->xMin = *std::min_element(xCord1.begin(), xCord1.end());
	this->yMax = *std::max_element(yCord1.begin(), yCord1.end());
	this->yMin = *std::min_element(yCord1.begin(), yCord1.end());
	this->zMax = *std::max_element(zCord1.begin(), zCord1.end());
	this->zMin = *std::min_element(zCord1.begin(), zCord1.end());
	for (int i = 0; i < points.size(); i++) {

		//glm::vec3 localCord = glm::vec3(points[i].x - this->massCenter.x,
		//	points[i].y - this->massCenter.y, points[i].z - this->massCenter.z);
		glm::vec3 localCord = points[i];
		this->localPos.push_back(localCord);

	}
	this->massCenter = glm::vec3(0);
}

std::vector<glm::vec3> Pedal::localToWorld(glm::vec3 massCenter, glm::mat3 rotMat) {

	std::vector<glm::vec3> worldPos;
	for (int i = 0; i < points.size(); i++) {
		worldPos.push_back(massCenter + rotMat * this->localPos[i]);
	}

	return worldPos;
}
void Pedal::updateThreeDegree() {
	std::vector<GLfloat> xCord1;
	std::vector<GLfloat> yCord1;
	std::vector<GLfloat> zCord1;
	for (int i = 0; i < points.size(); i++) {
		xCord1.push_back(this->points[i].x);
		yCord1.push_back(this->points[i].y);
		zCord1.push_back(this->points[i].z);
	}
	//get the three size of the rigid body
	this->xMax = *std::max_element(xCord1.begin(), xCord1.end());
	this->xMin = *std::min_element(xCord1.begin(), xCord1.end());
	this->yMax = *std::max_element(yCord1.begin(), yCord1.end());
	this->yMin = *std::min_element(yCord1.begin(), yCord1.end());
	this->zMax = *std::max_element(zCord1.begin(), zCord1.end());
	this->zMin = *std::min_element(zCord1.begin(), zCord1.end());
}


void Pedal::restoreDefault() {
	this->color = glm::vec3(1, 0.3, 0.0);
	this->model = glm::mat4(1);
	this->rotMat = glm::mat3(1);	
	this->massCenter = glm::vec3(0, 0, 0);
	this->elastic = 0.1f;
}
void Pedal::moveStick(GLfloat deltaTime) {
	//std::cout << "moving" << std::endl;
	if (flip) {

		if (this->moveDegree <= moveLimit && this->moveDegree >= 0) {

			this->setRotationMatrix((moveLimit / moveDuration) * deltaTime, glm::vec3(0,0,-1));
			this->moveDegree = this->moveDegree + (moveLimit / moveDuration) * deltaTime;
		}
		else if (this->moveDegree > moveLimit) {

			this->moveDegree = moveLimit;
			this->setState(2);
		}
		else if (this->moveDegree < 0) {
			this->moveDegree = 0;
			this->setState(0);
		}
	
	}
	else {
		if (this->moveDegree <= moveLimit && this->moveDegree >= 0) {

			this->setRotationMatrix((moveLimit / moveDuration) * deltaTime, glm::vec3(0, 0, 1));
			this->moveDegree = this->moveDegree + (moveLimit / moveDuration) * deltaTime;
		}
		else if (this->moveDegree > moveLimit) {

			this->moveDegree = moveLimit;
			this->setState(2);
		}
		else if (this->moveDegree < 0) {
			this->moveDegree = 0;
			this->setState(0);
		}
	}

}
GLfloat Pedal::GetAngVel()
{
	if (this->state == 0) {
		return 3;
	}
	else if (this->state == 1) {
		return 50;
	}
	else if (this->state == 2) {
		return 3;
	}
	else if (this->state == 4) {
		return 3;
	}
}