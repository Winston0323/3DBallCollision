#include "PointCloud.h"


PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
	: pointSize(pointSize)
{
	this->color = glm::vec3(0.3, 0.3, 0.4);
	/*
	 * TODO: Section 2: Currently, all the points are hard coded below.
	 * Modify this to read points from an obj file.
	 */
	objParser(objFilename);

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen.
	 */
	std::vector<GLfloat> xCord;
	std::vector<GLfloat> yCord;
	std::vector<GLfloat> zCord;
	for (int i = 0; i < points.size(); i++) {

		xCord.push_back(points[i].x);
		yCord.push_back(points[i].y);
		zCord.push_back(points[i].z);

	}
	GLfloat midX = (*std::max_element(xCord.begin(), xCord.end()) + *std::min_element(xCord.begin(), xCord.end())) / 2;
	GLfloat midY = (*std::max_element(yCord.begin(), yCord.end()) + *std::min_element(yCord.begin(), yCord.end())) / 2;
	GLfloat midZ = (*std::max_element(zCord.begin(), zCord.end()) + *std::min_element(zCord.begin(), zCord.end())) / 2;
	GLfloat maxDist = 0;
	this->center = glm::vec3(midX, midY, midZ);
	glm::vec3 maxPoint(0, 0, 0);
	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x - midX;
		points[i].y = points[i].y - midY;
		points[i].z = points[i].z - midZ;
		GLfloat temp = pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2);
		if (temp > maxDist) {
			maxDist = temp;
			maxPoint = points[i];
		}
	}
	maxDist = sqrt(maxDist);

	for (int i = 0; i < points.size(); i++) {

		points[i].x = points[i].x / maxDist;
		points[i].y = points[i].y / maxDist;
		points[i].z = points[i].z / maxDist;

	}


	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	//set the shade for light to default value
	shadeForLight = false;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
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

PointCloud::~PointCloud()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBOvertex);
	glDeleteBuffers(1, &VBOnormal);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}

void PointCloud::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// actiavte the shader program 
	glUseProgram(shader);
	glDisable(GL_CULL_FACE);
	model = tmodel * rmodel * smodel;
	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	//glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT,0);
	glDrawElements(GL_TRIANGLES, faces.size()* 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloud::update()
{
	this->scale(1.00001);
	this->spin(0.1f,glm::vec3(-1,0,0));
	//this->translation(glm::vec3(10,10,0));
}

void PointCloud::updatePointSize(GLfloat size)
{
	int afterSize = size + this->pointSize;
	if (afterSize < 1) {
		this->pointSize = 1;
	}
	else {
		this->pointSize = afterSize;
	}
}

void PointCloud::spin(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	rmodel = glm::rotate(glm::radians(angle), axis) * rmodel;

}
void PointCloud::scale(float level)
{
	smodel = glm::scale(glm::mat4(1.0f), glm::vec3(level)) * smodel;
}
void PointCloud::translation(glm::vec3 destination)
{
	tmodel = glm::translate(glm::mat4(1.0f), destination);
}

void PointCloud::objParser(string objFilename)
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

void PointCloud::setCenter(glm::vec3 center)
{
	this->center = center;
}
glm::vec3 PointCloud::getCenter()
{
	return this->center;
}