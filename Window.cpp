////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Cloth_Simulator";
bool Window::simStart = false;
bool Window::RK4 = true;
bool Window::launching = false;
bool Window::launch = false;

//time
GLfloat Window::speed = 0.0f;
GLfloat Window::lastFrameTime = 0.0f;
GLfloat Window::thisFrameTime = 0.0f;
GLfloat Window::deltaTime = 0.0f;
GLfloat Window::calTime = 0.0f;
GLfloat Window::renderRate = DEFAULT_RENDRATE;
GLfloat Window::simulationRate = DEFAULT_SIMRATE;
GLfloat Window::renderTimeStep = 1.0f / renderRate;
GLfloat Window::renderTime = renderTimeStep;
GLfloat Window::simTimeStep = 1.0f / simulationRate;
// Objects to render
RigidBody* Window::test;
Table* Window::table;
Ball* Window::ball;
// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::pointShaderProgram;
GLuint Window::toonShaderProgram;
GLuint Window::phongShaderProgram;
string objFileName = "obj/amongus_astro_still.obj";
////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	// Create a shader program with a vertex shader and a fragment shader.
	pointShaderProgram = LoadShaders("shaders/pointShader.vert", "shaders/pointShader.frag");

	// Check the shader program.
	if (!pointShaderProgram)
	{
		std::cerr << "Failed to initialize point shader program" << std::endl;
		return false;
	}
	// Create a shader program with a vertex shader and a fragment shader.
	toonShaderProgram = LoadShaders("shaders/toonShader.vert", "shaders/toonShader.frag");

	// Check the shader program.
	if (!toonShaderProgram)
	{
		std::cerr << "Failed to initialize toon shader program" << std::endl;
		return false;
	}
	// Create a shader program with a vertex shader and a fragment shader.
	phongShaderProgram = LoadShaders("shaders/phongShader.vert", "shaders/phongShader.frag");

	// Check the shader program.
	if (!phongShaderProgram)
	{
		std::cerr << "Failed to initialize phong shader program" << std::endl;
		return false;
	}
	//lastFrameTime = glfwGetTime();
	return true;

}

bool Window::initializeObjects()
{
	// STEP 1 : Create new object
	test = new RigidBody(objFileName, 1);
	ball = new Ball();
	table = new Table(27,48, glm::vec3(0));
	ball->setCollider(table->getCollider());
	
	return true;
}

void Window::cleanUp()
{

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}
////////////////////////////////////////////////////////////////////////////////
// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////
void Window::idleCallback()
{

	//if (ball->GetPos().x > 22 && ball->GetPos().y < -10.5) {
	//	ball->changeColor(glm::vec3(1,0,0));
	//}
	//else {
	//	ball->changeColor(glm::vec3(0.3, 0.3, 0.4));
	//}
	//STEP 2 update
	if (simStart) {
		//calculate time
		thisFrameTime = glfwGetTime();
		deltaTime = thisFrameTime - lastFrameTime;
		lastFrameTime = thisFrameTime;
		calTime += deltaTime;
		renderTime = renderTime - deltaTime;
		//when reaching the render time
		if (renderTime < EPSILON) {//STEP 2.2 render update 
			//renderUpdate here
			renderTime = renderTimeStep - renderTime;//reset timer
			ball->renderUpdate();
			//table->update(1 / DEFAULT_SIMRATE);
		}
		//Perform any updates as necessary.
		while (calTime > simTimeStep) {
			calTime -= simTimeStep;
			GLfloat restTime = 0.0f;
			//STEP 2.1 update 
			//table->update(simTimeStep);
			ball->update(simTimeStep,restTime);
			calTime += restTime;

			if (launching) {
				if (speed < 150) {
					speed = speed + deltaTime * 150;
					//std::cout << "launching" << std::endl;
				}
			}
			if (ball->GetPos().x > 22 && ball->GetPos().y < -10.5) {
				ball->changeColor(glm::vec3(1, 0, 0));
				if (launch == true) {
					std::cout << "speed is"<< speed << std::endl;
					ball->setVelocityX(speed);
					//ball->changeColor(glm::vec3(0, 0, 1));
					speed = 0;
					launch = false;
					launching = false;
				}
			}
			else {
				ball->changeColor(glm::vec3(0.3, 0.3, 0.4));
				if (table->GetClearTime() > 0) {
					ball->setGravMult(-10);
					ball->changeColor(glm::vec3(0.1, 0.2, 1));
				}
				else {
					ball->setGravMult(1);
					ball->changeColor(glm::vec3(0.3, 0.3, 0.4));
				}
			}
			if (table->rewardStatus()) {
				Cam->setStartFlip(true);
			}

		}
		table->update(simTimeStep);
	}
	else {//STEP 2.3 renderupdate when simulation not started  

		lastFrameTime = glfwGetTime();
		ball->renderUpdate();
		table->update(1/DEFAULT_SIMRATE);
	}

	
	//STEP 2.4 always update values
	Cam->Update(deltaTime);
	

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// STEP3 Render the object.
	//test->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
	table->draw(Cam->GetViewProjectMtx(), Window::shaderProgram, Window::pointShaderProgram);
	ball->draw(Cam->GetViewProjectMtx(), Window::shaderProgram, Window::pointShaderProgram);
	drawGUI();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}
////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;
		case GLFW_KEY_A:
			
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_LEFT:
			std::cout << "Pressing" << std::endl;
			table->getPedalLeft()->setState(1);
			break;
		case GLFW_KEY_RIGHT:
			std::cout << "Pressing" << std::endl;
			table->getPedalRight()->setState(1);
			break;

		case GLFW_KEY_SPACE:
			std::cout << "update speed" << std::endl;
			speed = 0;
			launching = true;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
	
		switch (key)
		{
		case GLFW_KEY_D:
			Cam->setStartFlip(true);
			break;
		case GLFW_KEY_LEFT:
			std::cout << "Releasing" << std::endl;
			table->getPedalLeft()->setState(3);
			break;
		case GLFW_KEY_RIGHT:
			std::cout << "Releasing" << std::endl;
			table->getPedalRight()->setState(3);
			break;

		case GLFW_KEY_SPACE:
			std::cout << "update speed" << std::endl;
			launching = false;
			launch = true;
		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse) return;
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;
	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
void Window::drawGUI() {

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImGui::GetIO().WantCaptureMouse = true;
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//Windspeed show
	ImGui::Begin("Pin Ball Engine");                          // Create a window called "Hello, world!" and append into it.
	
	ImGui::SetWindowSize(ImVec2(350, Window::height/2));
	ImGui::SetWindowPos(ImVec2(Window::width -350,0));

	//if (ImGui::Button("Change Intergrate Method")) {
	//}
	//ImGui::Text("Intergration technique : %s ", RK4 ? "RK4":"Eular");

	//if (ImGui::TreeNode("Spring Damping system")) {
	//	ImGui::SliderFloat("Spring Constant", cloth->getSpring(), 50, 2500);
	//	ImGui::SliderFloat("Damping Constant", cloth->getDamp(), 0, 50);
	//	ImGui::TreePop();
	//}
	if (ImGui::TreeNode("PIN BALL")) {
		ImGui::Text("Center:");
		ImGui::SliderFloat("X degrees", ball->getPosX(), -24, 24);
		ImGui::SliderFloat("Y degrees", ball->getPosY(), -24, 24);
		ImGui::Text("Velocity:");
		ImGui::SliderFloat("Vel X", ball->getVelX(), -500, 100);
		ImGui::SliderFloat("Vel Y", ball->getVelY(), -100, 100);
		ImGui::Text("Properties:");
		ImGui::SliderFloat("Elastic", ball->getElastic(), 0, 1);
		ImGui::SliderFloat("Raidus", ball->getSphereRadius(), 0.5, 2);
		ImGui::TreePop();
	}
	int bbindex = 1;
	if (ImGui::TreeNode("Bounce balls")) {
		for (BounceBall* bb : table->getBounceBalls()) {
			std::string name = "Bounce Ball" + std::to_string(bbindex);
			if (ImGui::TreeNode(name.c_str())) {
				ImGui::Text("Center:");
				ImGui::SliderFloat("X degrees", bb->getOriginX(), -24, 24);
				ImGui::SliderFloat("Y degrees", bb->getOriginY(), -24, 24);
				ImGui::Text("Properties:");
				ImGui::SliderFloat("Elastic", bb->getElastic(), 0, 1);
				ImGui::SliderFloat("Raidus", bb->getSphereRadius(), 0.5, 2);
				ImGui::TreePop();
			}
			bbindex++;
		}
		ImGui::TreePop();
	}


	ImGui::End();
	ImGui::Begin("Scores");                          // Create a window called "Hello, world!" and append into it.

	ImGui::SetWindowSize(ImVec2(350, Window::height / 2));
	ImGui::SetWindowPos(ImVec2(0, Window::height-500));
	if (ImGui::Button("start")) {
		simStart = !simStart;
	}
	

	if (ImGui::Button("restore default")) {
		resetGame();
	}
	ImGui::Text("Speed:%f", speed);
	ImGui::End();
	//draw imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Window::resetGame() {
	Cam->restoreDefault();
	ball->restoreDefault();
	//simStart = false;
}