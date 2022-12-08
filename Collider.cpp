#include "Collider.h"


////////////////////////////////////////////////////////////////////////////////

Collider::Collider(glm::vec3 center, GLfloat radius,GLfloat elastic)
{
	this->center = center;
	this->radius = radius;
	this->elastic = elastic;
	this->state = 1;//1 is sphere, 2 is plain, 3 is edge
}
////////////////////////////////////////////////////////////////////////////////
Collider::Collider(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 norm, GLfloat elastic, bool oneWay)
{
	this->center = p1;
	if (oneWay) {
		this->state = 4;
	}
	else {
		this->state = 2;
	}
	
	this->elastic = elastic;
	positions = {
		p1,
		p2,
		p3
	};
	//calculate normal for triangle
	//this->norm = glm::normalize(glm::cross(positions[1] - positions[0],
	//	positions[2] - positions[0]));
	this->norm = norm;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
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
}
////////////////////////////////////////////////////////////////////////////////
Collider::Collider(GLfloat elastic, glm::vec3 anker, glm::vec3 peak)
{
	this->center = anker;
	this->peak = peak;
	this->state = 2;
	this->elastic = elastic;
	glm::vec3 p1 = glm::vec3(this->center.x, this->center.y, this->center.z - 2);
	glm::vec3 p2 = glm::vec3(this->center.x, this->center.y, this->center.z + 2);
	glm::vec3 p3 = peak;
	
	positions = {
		p1,
		p2,
		p3
	};
	//calculate normal for triangle
	//this->norm = glm::normalize(glm::cross(positions[1] - positions[0],
	//	positions[2] - positions[0]));
	this->norm = norm;

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
	
}
////////////////////////////////////////////////////////////////////////////////
Collider::Collider(glm::vec3 p1, glm::vec3 p2, GLfloat elastic)
{
	this->center = p1;
	this->state = 3;
	this->elastic = elastic;
	positions = {
		p1,
		p2,
	};
}
////////////////////////////////////////////////////////////////////////////////
Collider::~Collider()
{
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Collider::GetNorm(glm::vec3 pos) 
{
	glm::vec3 result;
	if (this->state == 1 ) {//when sphere
		result = glm::normalize(pos - this->center);
		return result;
	}
	else if(this->state == 2 || this->state == 6) { //when plain
		//std::cout << "Should not use this getnorm on plain!!!!" << std::endl;
		if (checkSide(pos)) {
			result = this->norm;
		}
		else {
			result = -this->norm;
		}
		
		return result;
	}
	else if (this->state == 3) {
		glm::vec3 a = this->positions[1] - this->positions[0];
		glm::vec3 b = pos - this->positions[1];
		glm::vec3 ba = glm::dot(glm::normalize(a), b) * glm::normalize(a);
		glm::vec3 bperp = b - ba;
		glm::vec3 result = glm::normalize(bperp);
		return result;
	
	}
	else if (this->state == 4) {
		return this->norm;
	}
}
std::pair<bool, GLfloat> Collider::checkHit(glm::vec3 pos, glm::vec3 nextPos,glm::vec3 vel, GLfloat rad){
	
	if (this->oneTime && this->hit) {
		return std::make_pair(false, -1);
	}
	if (this->state == 1||state == 5) {
		glm::vec3 temp = pos - this->center;
		GLfloat dotProduct = glm::dot(temp, vel);
		if (dotProduct > 0) {
			return std::make_pair(false, -1);
		}
		GLfloat distance = glm::length(this->center - nextPos);//distance between two object
		if (distance - this->radius - rad < EPSILON) {
			GLfloat colDist = this->radius + rad;
			GLfloat startDist = glm::length(this->center - pos);
			GLfloat portion = colDist / startDist;
			std::cout << "possible" << std::endl;
			return std::make_pair(true, 0);

		}
		else {
			return std::make_pair(false, -1);
		}
	}
	else if (this->state == 2||this->state ==4 || this->state == 6) {//when it is a plain

		if (state == 4) {
			if (checkSide(pos)==false) {
				return std::make_pair(false, -1);
			}
			GLfloat dir = glm::dot(vel, this->norm);
			if (dir > 0) {
				return std::make_pair(false, -1);
			}

		}

		GLfloat dist = glm::length(glm::dot(this->center - nextPos, this->norm));
		if (dist > rad + EPSILON) {
			return std::make_pair(false, -1);
		}

		//GLfloat thit = glm::dot((this->center - pos + this->norm * rad), this->norm) / glm::dot(vel, this->norm);
		GLfloat thit = (glm::dot((this->center - pos), this->norm) + rad + EPSILON) / glm::dot(vel, this->norm);
		//if (thit < (1 / DEFAULT_SIMRATE + EPSILON) && thit > 0) {

		glm::vec3 xhit = pos - this->norm * rad + thit * vel;
		glm::vec2 xhitTD;
		std::vector<glm::vec2> pjtPoint;
		//project to 2D
		if (std::abs(this->norm.x) > std::abs(this->norm.y) &&
			std::abs(this->norm.x) > std::abs(this->norm.z)) {
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].y, positions[i].z));
			}
			xhitTD = glm::vec2(xhit.y, xhit.z);

		}
		else if (std::abs(this->norm.y) > std::abs(this->norm.x) && std::abs(this->norm.y) > std::abs(this->norm.z))
		{
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].z, positions[i].x));
			}
			xhitTD = glm::vec2(xhit.z, xhit.x);
		}
		else if (std::abs(this->norm.z) > std::abs(this->norm.x) && std::abs(this->norm.z) > std::abs(this->norm.y))
		{
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].x, positions[i].y));
			}
			xhitTD = glm::vec2(xhit.x, xhit.y);
		}
		else {
			for (int i = 0; i < positions.size(); i++) {
				pjtPoint.push_back(glm::vec2(positions[i].y, positions[i].z));
			}
			xhitTD = glm::vec2(xhit.y, xhit.z);
		}
		//comput edge vector
		std::vector<glm::vec2> edgeVec;
		std::vector<glm::vec2> hitVec;
		std::vector<glm::mat2> matrix;
		GLfloat ref = 0;
		for (int i = 0; i < pjtPoint.size(); i++) {
			glm::vec2 edge;
			if (i == pjtPoint.size() - 1) {
				edge = pjtPoint[0] - pjtPoint[i];
			}
			else {
				edge = pjtPoint[i + 1] - pjtPoint[i];
			}

			glm::vec2 hit = xhitTD - pjtPoint[i];
			glm::mat2 mat;
			mat[0][0] = edge.x;
			mat[1][0] = edge.y;
			mat[0][1] = hit.x;
			mat[1][1] = hit.y;

			GLfloat det = glm::determinant(mat);
			if (det * ref < 0) {//different sign
				return std::make_pair(false, -1.0f);
			}
			if (det != 0) {
				ref = det;
			}
		}
		if (state != 6) {
			return std::make_pair(true, thit);
		}
		else{
			return std::make_pair(true, -100);
		}
		
		//comput 

	//}
	//else {
	//	return std::make_pair(false, -1.0f);
	//}
	//return std::make_pair(false, -1);
	}
	else if (state==3){
		glm::vec3 a = this->positions[1] - this->positions[0];
		glm::vec3 b = pos - this->positions[1];
		glm::vec3 ba = glm::dot(glm::normalize(a), b) * glm::normalize(a);
		glm::vec3 bperp = b - ba;
		GLfloat dist = glm::length(bperp);
		if (dist < rad) {//hit
			return  std::make_pair(true, 0);
		}
		else {//not hit
			return std::make_pair(false, -1);
		}
	}

}


bool Collider::checkSide(glm::vec3 pos) {
	glm::vec3 temp =pos - this->center;
	GLfloat dotResult = glm::dot(temp, this->norm);
	if (dotResult < 0) {
		return false;
	}
	else {

		return true;
	}

}
void Collider::spin(glm::vec3 referPnt,glm::vec3 peak) {
	//update point
	this->positions[0] = glm::vec3(this->center.x, this->center.y, this->center.z - 2);
	this->positions[1] = glm::vec3(this->center.x, this->center.y, this->center.z + 2);
	this->positions[2] = peak;

	//update normal
	this->norm = glm::normalize(glm::cross(positions[1] - positions[0],positions[2] - positions[0]));
}

//glm::vec3 Collider::velResp(glm::vec3 pos, GLfloat angularVel) {
//	glm::vec3 pa = pos - anker;
//	glm::vec3 projN = glm::dot(pa, this->norm) * this->norm;
//	GLfloat ra = glm::length(pa - projN);
//	GLfloat magnitude = ra * angularVel;
//	return magnitude * this->norm;
//
//}