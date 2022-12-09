#ifndef _TABLE_H_
#define _TABLE_H_

#include "core.h"
#include "Plain.h"
#include "Pedal.h"
#include "Reward.h"
#include "BounceBall.h"
#include "ColliderBall.h"

////////////////////////////////////////////////////////////////////////////////

class Table
{
private:
	int beforceColliderBall;
	bool drawing = false;
	bool clear = false;
	bool allToggle = false;
	GLfloat clearTime = 0.0f;
	GLfloat clearLimit = 15.0f;
	GLfloat size;
	GLfloat width;
	GLfloat length;
	glm::vec3 origin;
	Plain* tableTop;
	Plain* tableUp;
	Plain* tableBtm;
	Plain* tableLeft;
	Plain* tableRight;
	Plain* leftTube;
	Plain* leftTubeOp;
	Plain* wallIncline;
	Plain* pipeLid;
	Plain* wallInclineBtmLft;
	Plain* wallInclineBtmRit;
	BounceBall* bounceBall1;
	BounceBall* bounceBall2;
	BounceBall* bounceBall3;
	ColliderBall* colliderBall11;
	ColliderBall* colliderBall12;
	ColliderBall* colliderBall13;
	Pedal* pedalLeft;
	Pedal* pedalRight;
	Reward* reward;
	std::vector<Plain*> plains;
	std::vector<BounceBall*> bbs;
	std::vector<ColliderBall*> cbs;
	std::vector<Collider*> colliders;
	void addCollider(Plain* plain);


public:
	Table(GLfloat width, GLfloat length, glm::vec3 origin);
	~Table();
	void draw(const glm::mat4& viewProjMtx, GLuint shader, GLuint pShader);
	void update(GLfloat deltaTime);
	std::vector<Plain*> getPlains() { return this->plains; }
	std::vector<Collider*> getCollider() { return this->colliders; }
	std::vector<BounceBall*> getBounceBalls() { return this->bbs; }
	bool rewardStatus() { return this->reward->GetRewardHit(); }
	Pedal* getPedalLeft() { return this->pedalLeft; }
	Pedal* getPedalRight() { return this->pedalRight; }
	GLfloat GetClearTime() { return this->clearTime; }
	void removeBall(int i);
};

////////////////////////////////////////////////////////////////////////////////

#endif
