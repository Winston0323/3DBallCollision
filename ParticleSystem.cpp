#include "ParticleSystem.h"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         #include "ParticleSystem.h"


////////////////////////////////////////////////////////////////////////////////
ParticleSystem::ParticleSystem(GLfloat mass, 
	GLfloat gravMult, GLfloat lifeSpan)
{
	this->restoreDefault();
	this->mass = mass;
	this->gravMult = gravMult;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	this->lifeSpan = lifeSpan;
	this->model = glm::mat4(1);
	this->velMag = 0;

	this->positions.reserve(3000);
	this->accelerations.reserve(3000);
	this->velocitys.reserve(3000);

	this->color = glm::vec3(1.0f, 0.95f, 0.1f);

	this->defaultForces();
	this->RoundOffError = 0;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glDisable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(GLfloat mass,
	GLfloat gravMult, GLfloat lifeSpan, GLfloat velMag)
{
	this->restoreDefault();
	this->mass = mass;
	this->gravMult = gravMult;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	this->lifeSpan = lifeSpan;
	this->model = glm::mat4(1);
	this->velMag = velMag;
	this->pointSize = 1.0f;

	this->positions.reserve(3000);
	this->accelerations.reserve(3000);
	this->velocitys.reserve(3000);

	this->color = glm::vec3(1.0f, 0.95f, 0.1f);

	this->defaultForces();
	this->RoundOffError = 0;
	this->radius = 1;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glDisable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
////////////////////////////////////////////////////////////////////////////////

ParticleSystem::ParticleSystem(GLfloat mass, glm::vec3 color,
	GLfloat gravMult, GLfloat lifeSpan, GLfloat velMag)
{
	this->restoreDefault();
	this->mass = mass;
	this->gravMult = gravMult;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);
	this->lifeSpan = lifeSpan;
	this->model = glm::mat4(1);
	this->velMag = velMag;
	this->pointSize = 3.0f;

	this->positions.reserve(3000);
	this->accelerations.reserve(3000);
	this->velocitys.reserve(3000);

	this->color = glm::vec3(1.0f, 0.95f, 0.1f);

	this->defaultForces();
	this->RoundOffError = 0;
	this->radius = 1;
	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glDisable(GL_CULL_FACE);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
////////////////////////////////////////////////////////////////////////////////
ParticleSystem::~ParticleSystem()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::genParticles(GLfloat deltaTime) {
	int num = floor(deltaTime * crtRate);
	RoundOffError = RoundOffError + deltaTime * crtRate - num;
	if (RoundOffError > 1) {
		num++;
		RoundOffError = RoundOffError - 1;
	}
	for (int i = 0; i < num; i++) {
		//caculating initial space
		GLfloat initX = glm::gaussRand(initPos.x, (float)sqrt(posVarX));
		GLfloat initY = glm::gaussRand(initPos.y, (float)sqrt(posVarY));
		GLfloat initZ = glm::gaussRand(initPos.z, (float)sqrt(posVarZ));
		glm::vec3 iPos = glm::vec3(initX, initY, initZ);

		GLfloat velX = glm::gaussRand(velocity.x, (float)sqrt(velVarX));
		GLfloat velY = glm::gaussRand(velocity.y, (float)sqrt(velVarY));
		GLfloat velZ = glm::gaussRand(velocity.z, (float)sqrt(velVarZ));
		glm::vec3 iVel = glm::vec3(velX, velY, velZ);
		//Particle* particle = new Particle(initPos, initVel, lifeSpan,
		//									grav,airResist, 
		//									radius, elastic, friction, plain);
		//particles.push_back(particle);
		if (avaInd.empty() == false) {
			int ind = avaInd.back();
			avaInd.pop_back();
			positions[ind] = iPos;
			velocitys[ind] = iVel;
			lifeTimes[ind] = lifeSpan;
			isActive[ind] = true;
		}
		else {
			positions.push_back(iPos);
			velocitys.push_back(iVel);
			lifeTimes.push_back(lifeSpan);
			if (positions.size() != velocitys.size() || lifeTimes.size() != positions.size()) {
				std::cout << "Postion velocity lifetime size is not matching !!!\n";
			}
			isActive.push_back(true);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::genParticlesCircle(GLfloat deltaTime, GLfloat radius) {
	int num = floor(deltaTime * crtRate);
	RoundOffError = RoundOffError + deltaTime * crtRate - num;
	if (RoundOffError > 1) {
		num++;
		RoundOffError = RoundOffError - 1;
	}
	for (int i = 0; i < num; i++) {
		//caculating initial space
		//GLfloat initX = glm::gaussRand(initPos.x, (float)sqrt(posVarX));
		//GLfloat initY = glm::gaussRand(initPos.y, (float)sqrt(posVarY));
		//GLfloat initZ = glm::gaussRand(initPos.z, (float)sqrt(posVarZ));

		glm::vec3 init = glm::ballRand(radius);
		init = glm::vec3(initPos.x + init.x, initPos.y + init.y, initPos.z + init.z);
		glm::vec3 iPos = init;


		if (avaInd.empty() == false) {
			int ind = avaInd.back();
			avaInd.pop_back();
			positions[ind] = iPos;
			velocitys[ind] = this->velocity;
			lifeTimes[ind] = lifeSpan;
			isActive[ind] = true;
		}
		else {
			positions.push_back(iPos);
			velocitys.push_back(this->velocity);
			lifeTimes.push_back(lifeSpan);
			if (positions.size() != velocitys.size() || lifeTimes.size() != positions.size()) {
				std::cout << "Postion velocity lifetime size is not matching !!!\n";
			}
			isActive.push_back(true);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::genParticlesCircle(GLfloat deltaTime, glm::vec3 start, glm::vec3 end) {
	//printf("start at %f,%f,%f\n", start.x, start.y, start.z);
	//printf("end at %f,%f,%f\n", end.x, end.y, end.z);
	int num = floor(deltaTime * crtRate);
	RoundOffError = RoundOffError + deltaTime * crtRate - num;
	if (RoundOffError > 1) {
		num++;
		RoundOffError = RoundOffError - 1;
	}
	for (int i = 0; i < num; i++) {
		GLfloat xDir = end.x - start.x;
		GLfloat yDir = end.y - start.y;
		//printf("xDir is %f y Dir is %f", xDir, yDir);
		GLfloat k = yDir / xDir;
		GLfloat b = start.y - k * (start.x);
		GLfloat variant = (float)rand();
		//printf("variant at %f", variant);
		GLfloat x = start.x + variant*xDir;
		GLfloat y = k * x + b;
		GLfloat z = rand() * 4-2;
		glm::vec3 iPos = glm::vec3(x, y, z);
		//printf("generate at %f,%f,%f\n", x, y, z);
		if (avaInd.empty() == false) {
			int ind = avaInd.back();
			avaInd.pop_back();
			positions[ind] = iPos;
			velocitys[ind] = this->velocity;
			lifeTimes[ind] = lifeSpan;
			isActive[ind] = true;
		}
		else {
			positions.push_back(iPos);
			velocitys.push_back(this->velocity);
			lifeTimes.push_back(lifeSpan);
			if (positions.size() != velocitys.size() || lifeTimes.size() != positions.size()) {
				std::cout << "Postion velocity lifetime size is not matching !!!\n";
			}
			isActive.push_back(true);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::cleanParticles()
{
	//clean particles
	for (int i = 0; i < lifeTimes.size(); i++) {
		GLfloat currLifeTime = lifeTimes[i];
		bool currActive = isActive[i];
		if (currLifeTime <= 0 && currActive == true) {
			avaInd.push_back(i);
			positions[i] = glm::vec3(100,100,100);
			isActive[i] = false;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::update(GLfloat deltaTime, glm::vec3 pos,
	glm::vec3 vel, GLfloat radius)
{
	this->initPos = pos;
	//caclculate gravForce
	genParticlesCircle(deltaTime, radius);
	cleanParticles();
	
	//this->velocity = -vel;
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);

	//update position data 
	
	for (int i = 0; i < positions.size(); i++) {
		if (isActive[i] == true) {
			integration(i, deltaTime, vel);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);

		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::update(GLfloat deltaTime, glm::vec3 pos, glm::vec3 vel)
{
	//caclculate gravForce
	genParticles(deltaTime);
	cleanParticles();
	this->initPos = pos;
	if (velMag != 0) {
		this->velocity = -glm::normalize(vel) * velMag;
	}
	else {
		this->velocity = -vel;
	}
	
	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);

	//update position data 

	for (int i = 0; i < positions.size(); i++) {
		if (isActive[i] == true) {
			integration(i, deltaTime, vel);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::update(GLfloat deltaTime,glm::vec3 start, glm::vec3 end, glm::vec3 vel)
{
	//caclculate gravForce
	
	genParticlesCircle(deltaTime, start, end);
	cleanParticles();


	this->gravForce = glm::vec3(0, -this->mass * this->grav * this->gravMult, 0);

	//update position data 

	for (int i = 0; i < positions.size(); i++) {
		if (isActive[i] == true) {
			integration(i, deltaTime, vel);
			//positions[i] = glm::vec3(positions[i].x, positions[i].y - 0.01, positions[i].z);
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// Actiavte the shader program 
	renderUpdate();
	glUseProgram(shader);
	//printf("Color is %f,%f,%f\n", this->color.x, this->color.y, this->color.z);
	// Get the shader variable locations and send the uniform data to the shader 
	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(this->color));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(this->pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, positions.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void ParticleSystem::integration(int index, GLfloat timeStep, glm::vec3 velocity) {

	//get and check next state
	glm::vec3 acc = this->force / this->mass;
	//calculate x(n+1)
	glm::vec3 nextPos = this->positions[index] + velocitys[index] * timeStep;
	glm::vec3 nextVel = velocitys[index] + acc * timeStep;

	this->positions[index] = nextPos;
	this->velocitys[index] = velocity * this->velMag;
	this->lifeTimes[index] = this->lifeTimes[index] - timeStep;
}
////////////////////////////////////////////////////////////////////////////////

void ParticleSystem::defaultForces()
{
	this->force = gravForce - this->airResist * this->velocity + this->airResist * this->windVelocity;
}
void ParticleSystem::renderUpdate()
{
	if (positions.size() != 0) {
		// Bind VBO to the bound VAO, and store the point data
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
		// Enable Vertex Attribute 0 to pass point data through to the shader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Unbind the VBO/VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
////////////////////////////////////////////////////////////////////////////////
void ParticleSystem::restoreDefault()
{
	elastic = DEFAULT_ELASTIC;
	airResist = DEFAULT_AIRRESIST;

	crtRate = 10;
	lifeSpan = 0.5;
	lastLifeSpan = lifeSpan;
	grav = GRAV_COE;
	gravMult = 0;
	mass = DEFAULT_MASS;

	radius = DEFAULT_RADIUS;
	friction = DEFAULT_FRICTION;


	initPos = glm::vec3(0.0f, 20.0f, 0.0f);
	velocity = glm::vec3(0.0f, -1.0f, 0.0f);
	windVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	posVarX = 1;
	posVarY = 1;
	posVarZ = 1;
	velVarX = 0;
	velVarY = 0;
	velVarZ = 0;
}