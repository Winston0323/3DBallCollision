#include "Table.h"
////////////////////////////////////////////////////////////////////////////////
Table::Table(GLfloat width,GLfloat length, glm::vec3 origin)
{
	this->width = width;
	this->length = length;
	this->origin = origin;

	//Table top
	glm::vec3 tableOrigin = glm::vec3(origin);
	tableTop = new Plain(width, length, tableOrigin ,glm::vec3(0, 0, 1), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableTop);

	//Table top
	glm::vec3 tableUpOrigin = glm::vec3(this->origin.x,this->origin.y + width/2, this->origin.z);
	tableUp = new Plain(4, length, tableUpOrigin, glm::vec3(0, -1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableUp);
	addCollider(tableUp);
	//Table bottom
	glm::vec3 tableBtmOrigin = glm::vec3(this->origin.x, this->origin.y - width / 2, this->origin.z);
	tableBtm = new Plain(4, length, tableBtmOrigin, glm::vec3(0, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableBtm);
	addCollider(tableBtm);
	//Table left
	glm::vec3 tableLftOrigin = glm::vec3(this->origin.x - length/2 , this->origin.y, this->origin.z);
	tableLeft = new Plain(width, 4, tableLftOrigin, glm::vec3(1, 0, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableLeft);
	addCollider(tableLeft);
	//Table leftTube
	glm::vec3 tableDownTubeOrigin = glm::vec3(this->origin.x+2.5 , this->origin.y -width/2+3, this->origin.z);
	leftTube = new Plain(4, length-5, tableDownTubeOrigin, glm::vec3(0, -1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(leftTube);
	addCollider(leftTube);

	leftTubeOp = new Plain(4, length - 5, tableDownTubeOrigin, glm::vec3(0, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(leftTubeOp);
	addCollider(leftTubeOp);
	//Table right
	glm::vec3 tableRHTOrigin = glm::vec3(this->origin.x + length / 2, this->origin.y-width/2+1.5, this->origin.z);
	tableRight = new Plain(3, 4, tableRHTOrigin, glm::vec3(-1, 0, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableRight);
	addCollider(tableRight);
	//wall left corner incline
	glm::vec3 wallInclineOrigin = glm::vec3(this->origin.x- length / 2 + 2.5, this->origin.y - width / 2 + 1, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallIncline = new Plain(4,5,4,wallInclineOrigin, true ,glm::vec3(0.9f, 0.9f, 0.5f),false);
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
	wallInclineBtmLft = new Plain(4, 2, 5, wallInclineBtmLftOrigin, false, glm::vec3(0.9f, 0.9f, 0.5f),false);
	plains.push_back(wallInclineBtmLft);
	addCollider(wallInclineBtmLft);

	glm::vec3 wallInclineBtmRitOrigin = glm::vec3(this->origin.x + length / 2 - 2, this->origin.y + width / 2 - 2.5, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallInclineBtmRit= new Plain(4, 2, 5, wallInclineBtmRitOrigin, true, glm::vec3(0.9f, 0.9f, 0.5f), false);
	plains.push_back(wallInclineBtmRit);
	addCollider(wallInclineBtmRit);

	
	//Bounce balls
	GLfloat upperGroupCenterX = -10;
	GLfloat upperGroupCenterY = +2;
	glm::vec3 ballOrigin1 = glm::vec3(upperGroupCenterX-8, upperGroupCenterY,0);
	bounceBall1 = new BounceBall(ballOrigin1, 2);
	bbs.push_back(bounceBall1);
	colliders.push_back(bounceBall1->GetCollider());

	//Bounce balls
	glm::vec3 ballOrigin2 = glm::vec3(upperGroupCenterX, upperGroupCenterY+6, 0);
	bounceBall2 = new BounceBall(ballOrigin2, 2);
	bbs.push_back(bounceBall2);
	colliders.push_back(bounceBall2->GetCollider());
	
	//Bounce balls
	glm::vec3 ballOrigin3 = glm::vec3(upperGroupCenterX, upperGroupCenterY - 6, 0);
	bounceBall3 = new BounceBall(ballOrigin3, 2);
	bbs.push_back(bounceBall3);
	colliders.push_back(bounceBall3->GetCollider());
	
	//pedals
	pedalLeft = new Pedal(glm::vec3(this->origin.x + length / 2-1, this->origin.y - width / 2 + 3 + 5, this->origin.z),0,false);
	pedalLeft->setRotationMatrix(-15.0f, glm::vec3(0,0,1));
	colliders.push_back(pedalLeft->GetCollider()[0]);
	colliders.push_back(pedalLeft->GetCollider()[1]);
	//colliders.push_back(pedalLeft->GetCollider()[1]);
	//pedalLeft = new Pedal(glm::vec3(0),0,true);
	pedalRight = new Pedal(glm::vec3(this->origin.x + length / 2 - 1, this->origin.y + width / 2 - 5, this->origin.z), 0, true);
	pedalRight->setRotationMatrix(15.0f, glm::vec3(0, 0, 1));
	colliders.push_back(pedalRight->GetCollider()[0]);
	colliders.push_back(pedalRight->GetCollider()[1]);
}

////////////////////////////////////////////////////////////////////////////////

Table::~Table()
{

}

////////////////////////////////////////////////////////////////////////////////
void Table::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	for (Plain* p : plains) {
		p->draw(viewProjMtx, shader);
	}
	for (BounceBall* bb : bbs) {
		bb->draw(viewProjMtx, shader);
	}
	pedalLeft->draw(viewProjMtx, shader);
	pedalRight->draw(viewProjMtx, shader);
}
void Table::update(GLfloat deltaTime) {
	//for (Plain* p : plains) {
	//	p->renderUpdate();
	//
	//}
	for (BounceBall* bb : bbs) {
		bb->renderUpdate();
	}
	pedalLeft->update(deltaTime);
	pedalRight->update(deltaTime);

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