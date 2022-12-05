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
	glm::vec3 tableLftTubeOrigin = glm::vec3(this->origin.x - length / 2 + 2.5f, this->origin.y -4, this->origin.z);
	leftTube = new Plain(width-8, 4, tableLftTubeOrigin, glm::vec3(1, 0, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(leftTube);
	addCollider(leftTube);
	//Table right
	glm::vec3 tableRHTOrigin = glm::vec3(this->origin.x + length / 2, this->origin.y, this->origin.z);
	tableRight = new Plain(width, 4, tableRHTOrigin, glm::vec3(-1, 0, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(tableRight);
	addCollider(tableRight);
	//wall left corner incline
	glm::vec3 wallInclineOrigin = glm::vec3(this->origin.x- length / 2 + 1, this->origin.y + width / 2 - 1, this->origin.z);
	//wallIncline = new Plain(4, 4, wallInclineOrigin, glm::vec3(1, 1, 0), glm::vec3(0.9f, 0.9f, 0.5f));
	wallIncline = new Plain(5,4,10,wallInclineOrigin, false ,glm::vec3(0.9f, 0.9f, 0.5f));
	plains.push_back(wallIncline);
	addCollider(wallIncline);
	//Bounce balls
	glm::vec3 ballOrigin1 = glm::vec3(-18,10,0);
	bounceBall1 = new BounceBall(ballOrigin1, 1.5);
	bbs.push_back(bounceBall1);
	colliders.push_back(bounceBall1->GetCollider());
	
	//Bounce balls
	glm::vec3 ballOrigin2 = glm::vec3(2, -5, 0);
	bounceBall2 = new BounceBall(ballOrigin2, 1);
	bbs.push_back(bounceBall2);
	colliders.push_back(bounceBall2->GetCollider());
	
	//Bounce balls
	glm::vec3 ballOrigin3 = glm::vec3(0.5, -3, 0);
	bounceBall3 = new BounceBall(ballOrigin3, 1);
	bbs.push_back(bounceBall3);
	colliders.push_back(bounceBall3->GetCollider());
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
}
void Table::update() {
	//for (Plain* p : plains) {
	//	p->renderUpdate();
	//
	//}
	for (BounceBall* bb : bbs) {
		bb->renderUpdate();
	
	}

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