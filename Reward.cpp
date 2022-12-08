#include "Reward.h"
////////////////////////////////////////////////////////////////////////////////
Reward::Reward(glm::vec3 origin, GLfloat radius)
{
	this->origin = origin;
	this->radius = radius;
	this->elastic = DEFAULT_BALL_ELASTIC;
	this->cakeR = new PointCloud(cake,1);
	this->extendR = new PointCloud(extend,1);
	this->magnetR = new PointCloud(magnet,1);
	this->extraPointR = new PointCloud(extraPoint,1);
	this->extraLifeR = new PointCloud(extraLife,1);
	this->cakeR->spin(90,glm::vec3(0,0,1));
	this->cakeR->setColor(glm::vec3(0.168, 0.090, 0));
	this->extendR->spin(90,glm::vec3(0,0,1));
	this->collider = new Collider(origin, radius,this->elastic);
	this->collider->setState(5);
	this->living = false;
	states.push_back(cakeR);
	//states.push_back(extendR);
	//states.push_back(magnetR);
	//states.push_back(extraPointR);
	//states.push_back(extraLifeR);
	switchState();
}

////////////////////////////////////////////////////////////////////////////////

Reward::~Reward()
{
	delete represent;
	delete collider;
}
////////////////////////////////////////////////////////////////////////////////
void Reward::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	if (living) {
		represent->draw(viewProjMtx, shader);
	}
}
////////////////////////////////////////////////////////////////////////////////
void Reward::update(GLfloat deltaTime)
{
	represent->update();

	if (living) {
		if (this->collider->GetHit() == true) {

			if (increaseTime < increaseLimit) {
				represent->scale(1 + 0.5 * deltaTime);
				increaseTime += deltaTime;
			}
			else {
				increaseTime = 0;
				this->collider->setHit(false);
				this->die();
			}
		}
		contextTime += deltaTime;
		//std::cout << contextTime << std::endl;
		if (contextTime > contextLimit) {
			this->die();
			contextTime = 0;
			living = false;
		}

	}
	else {
		this->collider->setHit(false);
	}
}
void Reward::switchState() {
	int index = std::rand() % this->states.size();
	this->represent = states[index];
	this->represent->translation(origin);
	this->represent->clearScale();
}
void Reward::spawn() {
	std::cout << "spawn" << std::endl;
	this->living = true;  
	this->represent->clearScale();
}
void Reward::die() {
	std::cout << "die" << std::endl;
	this->living = false;
}
