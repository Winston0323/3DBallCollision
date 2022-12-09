#ifndef _REWARD_H_
#define _REWARD_H_

#include "core.h"
#include "Plain.h"
#include "Collider.h"
#include "PointCloud.h"

////////////////////////////////////////////////////////////////////////////////

class Reward
{
private:
	bool living;
	GLfloat elastic;
	GLfloat radius;
	GLfloat increaseTime = 0.0f;
	GLfloat increaseLimit = 5.0f;
	GLfloat contextTime = 0.0f;
	GLfloat contextLimit = 30.0f;
	glm::vec3 origin;
	PointCloud* represent;
	PointCloud* cakeR;
	PointCloud* extendR;
	PointCloud* magnetR;
	PointCloud* extraPointR;
	PointCloud* extraLifeR;
	Collider* collider;
	std::string cake = "obj/cake.obj";
	std::string extend = "obj/cone.obj";
	std::string magnet = "obj/magnet.obj";
	std::string extraPoint = "obj/coin.obj";
	std::string extraLife = "obj/sphere.obj";
	std::string currFile = extend;
	std::vector<PointCloud* > states;
	


public:
	Reward(glm::vec3 origin, GLfloat radius );
	~Reward();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void update(GLfloat deltaTime);
	//getters
	glm::vec3 GetOrigin()	{ return origin; }
	GLfloat GetElastic()	{ return this->elastic; }
	GLfloat GetRadius()		{ return this->radius; }
	Collider* GetCollider() { return this->collider; }
	bool GetLiving() { return this->living; }
	bool GetRewardHit() { return this->living && this->collider->GetHit(); }

	//getter for imgui
	GLfloat* getOriginX()	{ return &this->origin.x; }
	GLfloat* getOriginY()	{ return &this->origin.y; }
	void spawn();
	void die();
	void switchState();
	void renderUpdate() { this->translate(this->origin); }
	void translate(glm::vec3 destination) {
		this->origin = destination;
		this->collider->translate(destination);
		this->represent->translation(destination);
	}
};

////////////////////////////////////////////////////////////////////////////////

#endif
