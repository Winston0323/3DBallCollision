#include "Ball.h"


////////////////////////////////////////////////////////////////////////////////

Ball::Ball()
{
	this->sphere = new Sphere(1.0, glm::vec3(0.3, 0.3, 0.4));
	restoreDefault();
	//gravity
	this->gravForce = glm::vec3(0, -mass * grav, 0);
	this->gravForceMult = glm::vec3(0, -mass * grav * gravMult, 0);

	//constant
	this->defaultForces();//initialize forces
	//representation
	
	std::vector<Plain*>p;
	//ps = new ParticleSystem(p, 1.0f, 0.1f, 3.0f, 0.5f);

}

////////////////////////////////////////////////////////////////////////////////
Ball::~Ball()
{
}
////////////////////////////////////////////////////////////////////////////////
void Ball::update(GLfloat timeStep, GLfloat& restTime)
{
	//this->partSys->update(timeStep);
	this->gravForceMult = glm::vec3(this->mass * this->grav * this->gravMult, 0, 0);
	boxCollision(timeStep, restTime);
	//ps->update(timeStep, this->GetPos(), this->GetVelocity(), this->GetRadius());

}
////////////////////////////////////////////////////////////////////////////////
void Ball::draw(const glm::mat4& viewProjMtx,  GLuint shader, GLuint pShader)
{
	sphere->draw(viewProjMtx, shader);
	//ps->draw(view, projection, pShader);
}
////////////////////////////////////////////////////////////////////////////////
void Ball::ApplyForce(glm::vec3 f)
{
	this->force = this->force + f;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::defaultForces() {
	this->force = gravForceMult - this->airResist * this->velocity;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::GetPos()
{
	return this->position;
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::GetVelocity()
{
	return this->velocity;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::addNorm(glm::vec3 normal) {
	this->norm = this->norm + normal;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::clearNorm() {
	this->norm = glm::vec3(0.0);
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::getNorm() {
	return this->norm;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::normalize() {
	this->norm = glm::normalize(this->norm);
}
////////////////////////////////////////////////////////////////////////////////
glm::vec3 Ball::getPos() {
	return this->position;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::translate(float dist) {
	this->position = this->position + glm::vec3(0.0f, dist, 0.0f);
}
////////////////////////////////////////////////////////////////////////////////
std::vector<glm::vec3> Ball::intRK(GLfloat timeStep) {
	//get and check next state
	glm::vec3 K1Acc = this->force / this->mass;
	//calculate x(n+1)
	glm::vec3 K1Pos = this->position + this->velocity* timeStep;
	glm::vec3 K1Vec = this->velocity + K1Acc * timeStep;

	//loop through all springs that the index is connected
	//glm::vec3 K2force = SumSpringForce(index, K1Pos, K1Vec) + SumAeroForce(index, K1Pos, K1Vec);
	glm::vec3 K2force = this->force / this->mass;
	glm::vec3 K2Acc = K2force / this->mass;
	glm::vec3 K2Pos = this->position + K1Vec * 0.5f * timeStep;
	glm::vec3 K2Vec = this->velocity+ K2Acc * 0.5f * timeStep;

	//loop through all springs that the index is connected
	glm::vec3 K3force = this->force / this->mass;
	glm::vec3 K3Acc = K3force / this->mass;
	glm::vec3 K3Pos = this->position + K2Vec * 0.5f * timeStep;
	glm::vec3 K3Vec = this->velocity + K3Acc * 0.5f * timeStep;

	//loop through all springs that the index is connected
	glm::vec3 K4force = this->force / this->mass;
	glm::vec3 K4Acc = K4force / this->mass;
	glm::vec3 K4Pos = this->position + K3Vec * timeStep;
	glm::vec3 K4Vec = this->velocity + K4Acc * timeStep;

	glm::vec3 nextPos = this->position +
		(timeStep / 6.0f) * (K1Vec + 2.0f * K2Vec + 2.0f * K3Vec + K4Vec);

	glm::vec3 nextVel = this->velocity +
		(timeStep / 6.0f) * (K1Acc + 2.0f * K2Acc + 2.0f * K3Acc + K4Acc);

	std::vector<glm::vec3> result;
	result.push_back(nextPos);
	result.push_back(nextVel);
	result.push_back(K4Acc);
	return result;
}
////////////////////////////////////////////////////////////////////////////////
void Ball::boxCollision(GLfloat timeStep, GLfloat& restTime) {

	//get and check next state
	std::vector<glm::vec3> result = this->intRK(timeStep);
	//calculate x(n+1)
	glm::vec3 nextPos = result[0];
	glm::vec3 nextVel = result[1];
	glm::vec3 acc = result[2];

	//detection
	std::vector<Collider*> hitColliders;
	GLfloat f = 1.0f;
	Collider* hitCollider;
	bool hit = false;
	int i = 0;
	#pragma omp parallel
	#pragma omp for
	for (int i = 0; i < this->colliders.size();i++) {
		Collider* collider = this->colliders[i];
		if (collider->checkHit(this->position, nextPos, 
				this->velocity,this->radius).first) {//check if hit
			if (collider->checkHit(this->position, nextPos, 
					this->velocity, this->radius).second < f) {//check if f is smaller than the current f
				f = collider->checkHit(this->position, nextPos,
					this->velocity, this->radius).second;//set f 
				hitCollider = collider;//set collider 
				hitCollider->setHit(true);
				hit = true;//set hit
			}
			
		}
		
	}
	//determination
	//when there is at least one plain collide
	if (hit&& hitCollider->GetState()!=5) {
		//position and velocity on collision
		//if (f < 0) {
		//	f = 0.000001;
		//}
		if (f == -100) {
			GLfloat yPos = 0;
			if (this->position.y < 1.5){
				yPos = 1.5 + (1.5 - position.y);
			}
			else {
				yPos = 1.5 - (position.y - 1.5);
			}
			this->position = glm::vec3(this->position.x, yPos, 0);
			std::cout << hitCollider->GetNorm().x <<hitCollider->GetNorm().y<<std::endl;
			glm::vec3 calNorm = glm::vec3(-hitCollider->GetNorm().x, hitCollider->GetNorm().y, 0);
			this->velocity = glm::dot(glm::vec3(-this->velocity.x,-this->velocity.y,0), -hitCollider->GetNorm()) * calNorm;
			return;
		}
		std::cout << "f is " << f << std::endl;
		glm::vec3 posCol = position + f * timeStep * velocity;
		glm::vec3 vecCol = velocity + f * timeStep * acc;
		restTime = (1 - f) * timeStep;
		//Response
		glm::vec3 nhat = hitCollider->GetNorm(posCol);
		//std::cout << "nhat is" << nhat.x << " ," << nhat.y << " ," << nhat.z << "\n";
		//seperate velocity
		glm::vec3 vn = glm::dot(vecCol, nhat) * nhat;
		glm::vec3 vt = vecCol - vn;
		//calculate next step velocity
		glm::vec3 vtnext;
		glm::vec3 vnnext = -(this->elastic + hitCollider->GetElastic()) * vn;//elasticity
		if (glm::length(vt) != 0) {
			vtnext = vt - std::min(this->friction * glm::length(vn), glm::length(vt)) * glm::normalize(vt);
		}
		else {
			vtnext = glm::vec3(0);
		}
		glm::vec3 vnext = vnnext + vtnext;

		//update position and velocity
		//this->position = posCol;
		if (hitCollider->GetState() == 1) {
			
			this->position = this->position + hitCollider->GetNorm(posCol) * 0.01f;
			this->velocity = vnext;
		}
		else if (hitCollider->GetState() == 2) {
			this->position = this->position + hitCollider->GetNorm(posCol) * 0.01f;
			this->velocity = vnext;
		}
		else if (hitCollider->GetState() == 3) {
		
			this->position = this->position + hitCollider->GetNorm(posCol) * 0.01f;
			this->velocity = vnext;
		}
		else {
			this->position = this->position + hitCollider->GetNorm(posCol) * 0.01f;
			this->velocity = vnext;
		}
		

		//this->force = gravForceMult;//clear force
		defaultForces();
	}
	else {//when no collision detected
		//update state
		this->position = nextPos;
		this->velocity = nextVel;
		//this->force = gravForceMult;//clear force
		defaultForces();
	}


	if (this->velocity.x* this->velocity.x+this->velocity.y* this->velocity.y > 2500) {
		if (this->position.y > -10.5) {
			//std::cout << "damped" << std::endl;
			glm::vec3 velDir = glm::normalize(this->velocity);
			this->velocity = velDir * 50.0f;
		}
	}
	this->position.z = 0;

}
////////////////////////////////////////////////////////////////////////////////
void Ball::renderUpdate()
{
	sphere->translation(this->position);
}

////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getGravMult()
{
	return &this->gravMult;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getAirResist()
{
	return &this->airResist;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat Ball::getRadius()
{
	return this->radius;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getFriction()
{
	return &this->friction;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getElastic()
{
	return &this->elastic;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getMass()
{
	return &this->mass;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosX()
{
	return &this->position.x;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosY()
{
	return &this->position.y;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getPosZ()
{
	return &this->position.z;
}
////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelX()
{
	return &this->velocity.x;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelY()
{
	return &this->velocity.y;
}
//////////////////////////////////////////////////////////////////////////////////
GLfloat* Ball::getVelZ()
{
	return &this->velocity.z;
}

////////////////////////////////////////////////////////////////////////////////
void Ball::restoreDefault()
{
	this->position = glm::vec3(+21.5,-12,0);
	this->velocity = glm::vec3(0);
	this->windVelocity = glm::vec3(0);
	this->mass = DEFAULT_MASS;
	this->elastic = DEFAULT_ELASTIC;
	this->airResist = DEFAULT_AIRRESIST;
	this->gravMult = DEFAULT_GRAVMULT;
	this->radius = DEFAULT_RADIUS;
	*this->getSphereRadius() = DEFAULT_RADIUS;
	this->friction = DEFAULT_FRICTION;
	
	this->sphere->changeColor(glm::vec3(0.3, 0.3, 0.4));
	this->grav = DEFAULT_GRAVCONST;
	this->gravForce = glm::vec3(0, -mass * grav, 0);
	this->gravForceMult = glm::vec3(0, -mass * grav * gravMult, 0);
	//initPos = glm::vec3(0.0f, 10.0f, 0.0f);
	//velocity = glm::vec3(0.0f, -1.0f, 0.0f);
}