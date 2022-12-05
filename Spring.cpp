#include "Spring.h"


////////////////////////////////////////////////////////////////////////////////

Spring::Spring(glm::vec3 center,glm::vec3 direction,
	GLfloat SConst, GLfloat RLength)
{
	GLfloat offset = RLength / 2;
	this->pos1 = center + direction * offset;
	this->pos2 = center - direction * offset;
	this->springConst = SConst;
	this->restLength = RLength;
	//recording restPos
	p1StartPos = pos1;
	p2StartPos = pos2;

}

////////////////////////////////////////////////////////////////////////////////

Spring::~Spring()
{

}

////////////////////////////////////////////////////////////////////////////////
void Spring::ComputeForce() {
	
	//calculate spring force
	glm::vec3 p1CurPos = this->pos1;
	glm::vec3 p2CurPos = this->pos2;
	GLfloat currLength = glm::length(p2CurPos - p1CurPos);//calculate currLength

	//Hooke's law 
	glm::vec3 disPlcVec = (restLength - currLength) * glm::normalize(p2CurPos - p1CurPos);
	glm::vec3 fSpring = -(springConst) * disPlcVec;
	
	//caculating damper force
	//glm::vec3 p1CurVec = p1->GetVelocity();//collcet particle velocity
	//glm::vec3 p2CurVec = p2->GetVelocity();
	//float vClose = glm::dot(p1CurVec - p2CurVec , glm::normalize(p2CurPos - p1CurPos));
	//glm::vec3 fDamper = -dampingConst * vClose * glm::normalize(p2CurPos - p1CurPos);

	//apply force
	//glm::vec3 force = fSpring + fDamper;
	glm::vec3 force = fSpring;
	//p1->ApplyForce(force);
	//p2->ApplyForce(-force);
}
////////////////////////////////////////////////////////////////////////////////