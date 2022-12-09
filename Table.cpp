#include "Table.h"
////////////////////////////////////////////////////////////////////////////////
Table::Table(GLfloat width,GLfloat length, glm::vec3 origin)
{
	this->width = width;
	this->length = length;
	this->origin = origin;

	//Table top
	glm::vec3 wallColor = glm::vec3(0.701, 0.701, 0.8);
	glm::vec3 tableOrigin = glm::vec3(origin);
	tableTop = new Plain(width, length, tableOrigin ,glm::vec3(0, 0, 1), wallColor);
	plains.push_back(tableTop);

	//Table top
	glm::vec3 tableUpOrigin = glm::vec3(this->origin.x,this->origin.y + width/2, this->origin.z);
	tableUp = new Plain(4, length, tableUpOrigin, glm::vec3(0, -1, 0), wallColor);
	plains.push_back(tableUp);
	addCollider(tableUp);
	//Table bottom
	glm::vec3 tableBtmOrigin = glm::vec3(this->origin.x, this->origin.y - width / 2, this->origin.z);
	tableBtm = new Plain(4, length, tableBtmOrigin, glm::vec3(0, 1, 0), wallColor);
	plains.push_back(tableBtm);
	addCollider(tableBtm);
	//Table left
	glm::vec3 tableLftOrigin = glm::vec3(this->origin.x - length/2 , this->origin.y, this->origin.z);
	tableLeft = new Plain(width, 4, tableLftOrigin, glm::vec3(1, 0, 0), wallColor);
	plains.push_back(tableLeft);
	addCollider(tableLeft);
	//Table leftTube
	glm::vec3 tableDownTubeOrigin = glm::vec3(this->origin.x+2.5 , this->origin.y -width/2+3, this->origin.z);
	leftTube = new Plain(4, length-5, tableDownTubeOrigin, glm::vec3(0, -1, 0), wallColor);
	plains.push_back(leftTube);
	addCollider(leftTube);

	leftTubeOp = new Plain(4, length - 5, tableDownTubeOrigin, glm::vec3(0, 1, 0), wallColor);
	plains.push_back(leftTubeOp);
	addCollider(leftTubeOp);
	//Table right
	glm::vec3 tableRHTOrigin = glm::vec3(this->origin.x + length / 2, this->origin.y-width/2+1.5, this->origin.z);
	tableRight = new Plain(3, 4, tableRHTOrigin, glm::vec3(-1, 0, 0), wallColor);
	tableRight->getCollider()[0]->setElastic(-0.5);
	tableRight->getCollider()[1]->setElastic(-0.5);
	plains.push_back(tableRight);
	addCollider(tableRight);
	//wall left corner incline
	glm::vec3 wallInclineOrigin = glm::vec3(this->origin.x- length / 2 + 2.5, this->origin.y - width / 2 + 1, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallIncline = new Plain(4,5,4,wallInclineOrigin, true , wallColor,false);
	plains.push_back(wallIncline);
	addCollider(wallIncline);
	//pipe Lid
	glm::vec3 pipeLidOrigin = glm::vec3(this->origin.x - length / 2 + +2.5, this->origin.y - width / 2+5, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	pipeLid = new Plain(4, 5, 4, pipeLidOrigin, true, glm::vec3(0.678f, 0.847f, 1.0f),true);
	plains.push_back(pipeLid);
	addCollider(pipeLid);
	
	//Bottom incline walls
	glm::vec3 wallInclineBtmLftOrigin = glm::vec3(this->origin.x + length / 2-2, this->origin.y - width / 2 + 3 + 2.5, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallInclineBtmLft = new Plain(4, 2, 5, wallInclineBtmLftOrigin, false, wallColor,false);
	plains.push_back(wallInclineBtmLft);
	wallInclineBtmLft->getCollider()[0]->setState(6);
	wallInclineBtmLft->getCollider()[1]->setState(6);
	wallInclineBtmLft->setColor(glm::vec3(0, 0.2, 1));
	wallInclineBtmLft->setPartVel(glm::vec3(-2, 1, 0));
	addCollider(wallInclineBtmLft);

	glm::vec3 wallInclineBtmRitOrigin = glm::vec3(this->origin.x + length / 2 - 2, this->origin.y + width / 2 - 2.5, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallInclineBtmRit= new Plain(4, 2, 5, wallInclineBtmRitOrigin, true, wallColor, false);
	plains.push_back(wallInclineBtmRit);
	wallInclineBtmRit->getCollider()[0]->setState(6);
	wallInclineBtmRit->getCollider()[1]->setState(6);
	wallInclineBtmRit->setColor(glm::vec3(1, 0.603, 0));
	wallInclineBtmRit->setPartVel(glm::vec3(-2, -1, 0));
	addCollider(wallInclineBtmRit);

	//Bounce balls
	GLfloat upperGroupCenterX = -10;
	GLfloat upperGroupCenterY = +2;
	glm::vec3 ballOrigin1 = glm::vec3(upperGroupCenterX-8, upperGroupCenterY,0);
	bounceBall1 = new BounceBall(ballOrigin1, 2);
	bbs.push_back(bounceBall1);
	colliders.push_back(bounceBall1->GetCollider());

	//Bounce balls
	glm::vec3 ballOrigin2 = glm::vec3(upperGroupCenterX, upperGroupCenterY + 5 , 0);
	bounceBall2 = new BounceBall(ballOrigin2, 2);
	bbs.push_back(bounceBall2);
	colliders.push_back(bounceBall2->GetCollider());
	
	//Bounce balls
	glm::vec3 ballOrigin3 = glm::vec3(upperGroupCenterX, upperGroupCenterY - 5, 0);
	bounceBall3 = new BounceBall(ballOrigin3, 2);
	bbs.push_back(bounceBall3);
	colliders.push_back(bounceBall3->GetCollider());
	//pedals
	pedalLeft = new Pedal(glm::vec3(this->origin.x + length / 2 - 1, this->origin.y - width / 2 + 3 + 5, this->origin.z), 0, false);
	pedalLeft->setRotationMatrix(-15.0f, glm::vec3(0, 0, 1));
	colliders.push_back(pedalLeft->GetCollider()[0]);
	colliders.push_back(pedalLeft->GetCollider()[1]);
	//colliders.push_back(pedalLeft->GetCollider()[1]);
	//pedalLeft = new Pedal(glm::vec3(0),0,true);
	pedalRight = new Pedal(glm::vec3(this->origin.x + length / 2 - 1, this->origin.y + width / 2 - 5, this->origin.z), 0, true);
	pedalRight->setRotationMatrix(15.0f, glm::vec3(0, 0, 1));
	colliders.push_back(pedalRight->GetCollider()[0]);
	colliders.push_back(pedalRight->GetCollider()[1]);
	
	reward = new Reward(glm::vec3(upperGroupCenterX, upperGroupCenterY,0), 1);
	colliders.push_back(reward->GetCollider());
	this->beforceColliderBall = colliders.size();
	GLfloat distance = 4;
	int number = 5;
	GLfloat cBallCenterY = 1;
	GLfloat cBallCenterX = -8;
	for (int j = 0; j < number; j++) {
		//Collider balls
		cBallCenterX = cBallCenterX + distance;
		for (int i = 0; i <= number / 2; i = i + 1) {
			GLfloat diffX = (float)std::rand() / RAND_MAX * 2;
			GLfloat diffY = (float)std::rand() / RAND_MAX * 2;
			if (i == 0) {
				glm::vec3 currOrigin = glm::vec3(cBallCenterX+diffX, cBallCenterY + diffY, 0);
				colliderBall11 = new ColliderBall(currOrigin, 0.5);
				cbs.push_back(colliderBall11);
				colliders.push_back(colliderBall11->GetCollider());
			}
			else {

				glm::vec3 currOrigin = glm::vec3(cBallCenterX + diffX, cBallCenterY - distance * i+ diffY, 0);
				colliderBall11 = new ColliderBall(currOrigin, 0.5);
				cbs.push_back(colliderBall11);
				colliders.push_back(colliderBall11->GetCollider());
				currOrigin = glm::vec3(cBallCenterX + diffX, cBallCenterY + distance * i + diffY, 0);
				colliderBall11 = new ColliderBall(currOrigin, 0.5);
				cbs.push_back(colliderBall11);
				colliders.push_back(colliderBall11->GetCollider());
			}

		}
	}

}

////////////////////////////////////////////////////////////////////////////////

Table::~Table()
{

}

////////////////////////////////////////////////////////////////////////////////
void Table::draw(const glm::mat4& viewProjMtx, GLuint shader, GLuint pShader)
{
	for (Plain* p : plains) {
		p->draw(viewProjMtx, shader, pShader);
	}
	for (BounceBall* bb : bbs) {
		bb->draw(viewProjMtx, shader);
	}
	drawing = false;
	for (ColliderBall* cb : cbs) {
		if (!cb->GetCollider()->GetHit()) {
			cb->draw(viewProjMtx, shader);
			drawing = true;
		}
	}
	if (drawing == false) {
		clear = true;
	}
	pedalLeft->draw(viewProjMtx, shader);
	pedalRight->draw(viewProjMtx, shader);
	reward->draw(viewProjMtx, shader);
}
void Table::update(GLfloat deltaTime) {

	wallInclineBtmLft->update(deltaTime);
	wallInclineBtmRit->update(deltaTime);

	allToggle = true;
	for (BounceBall* bb : bbs) {
		bb->renderUpdate();
		bb->update(deltaTime);
		if (!bb->GetToggle()) {
			allToggle = false;
		}
	}
	if (allToggle && !reward->GetLiving()) {
		reward->spawn();
		for (BounceBall* bb : bbs) {
			bb->setToggle(false);
		}
	}
	if (clear == true) {
		clearTime = clearTime + deltaTime;
		if (clearTime > clearLimit) {
			for (ColliderBall* cb : cbs) {
				cb->GetCollider()->setHit(false);
				clearTime = 0.0f;
				clear = false;
			}
		}
	}
	//for (int i = 0; i < cbs.size();i++) {
	//	ColliderBall* cb = this->cbs[i];
	//	if (cb->GetCollider()->GetHit()) {
	//		removeBall(i);
	//	}
	//}
	
	pedalLeft->update(deltaTime);
	pedalRight->update(deltaTime);
	reward->update(deltaTime);
}
void Table::addCollider(Plain* plain) {
	//colliders.push_back(plain->getCollider()[0]);
	//colliders.push_back(plain->getCollider()[1]);
	//colliders.push_back(plain->getCollider()[2]);
	//colliders.push_back(plain->getCollider()[3]);
	for (Collider* collider : plain->getCollider()) {
		colliders.push_back(collider);
	}
}
void Table::removeBall(int i) {
	cbs.erase(cbs.begin()+i);
}

