////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "main.h"

////////////////////////////////////////////////////////////////////////////////

// The Camera class provides a simple means to controlling the 3D camera. It could
// be extended to support more interactive controls. Ultimately. the camera sets the
// GL projection and viewing matrices.

class Camera {
public:
	Camera();
	GLfloat flipTime = 0.0f;
	GLfloat flipLimit = 5.0f;
	bool startFlip = false;
	void Update(GLfloat deltaTime);
	void Reset();
	void restoreDefault();

	// Access functions
	void SetAspect(float a) { Aspect = a; }
	void SetDistance(float d) { Distance = d; }
	void SetAzimuth(float a) { Azimuth = a; }
	void SetIncline(float i) { Incline = i; }
	void setStartFlip(bool val) { this->startFlip = true; }
	float GetDistance() { return Distance; }
	float GetAzimuth() { return Azimuth; }
	float GetIncline() { return Incline; }
	glm::vec3 GetPos() { return camPos; }

	const glm::mat4& GetView() { return View; }
	const glm::mat4& GetProject() { return Project; }
	const glm::mat4& GetViewProjectMtx() { return ViewProjectMtx; }
	void flipping(GLfloat deltaTime) {
		
		if (flipTime > flipLimit) {
			std::cout << "stop flipping" << std::endl;
			this->SetAzimuth(0);
			flipTime = 0;
			this->startFlip = false;
		}
		else {
			
			if (startFlip) {
				std::cout << "start flipping" << flipTime << std::endl;
				this->flipTime += deltaTime;
				this->SetAzimuth(180);
			}
		}
	
	}
private:
	// Perspective controls
	float FOV;			// Field of View Angle (degrees)
	float Aspect;		// Aspect Ratio
	float NearClip;		// Near clipping plane distance
	float FarClip;		// Far clipping plane distance

	// Polar controls
	float Distance;		// Distance of the camera eye position to the origin (meters)
	float Azimuth;		// Rotation of the camera eye position around the Y axis (degrees)
	float Incline;		// Angle of the camera eye position over the XZ plane (degrees)

	glm::vec3 camPos;
	// Computed data
	glm::mat4 ViewProjectMtx;
	glm::mat4 View;
	glm::mat4 Project;
	
};

////////////////////////////////////////////////////////////////////////////////
