#pragma once

#include<iostream>

#include<glfw3.h>
#include<glm.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>

class Camera {
private:
	// View Matrix : Camera Position, Front and World Up
	glm::mat4 ViewMatrix;

	glm::vec3 worldUp;
	glm::vec3 position, front, up, right;

	int invertX = 1;
	int invertY = 1;
	
	GLfloat pitch, yaw, roll;
	GLfloat movementSpeed, sensivity;

	// Update Vectors using updated pitch and yaw in updateMouseInput
	void updateCameraVectors() {
		this->front.x = this->invertY * cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = this->invertX * sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

public:
	// Constructors
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp) {
		// Set Default Values
		this->ViewMatrix = glm::mat4(1.f);

		this->movementSpeed = 3.f;
		this->sensivity = 10.f;

		this->worldUp = worldUp;
		this->position = position;
		this->up = worldUp;
		this->right = glm::vec3(0.f);

		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;

		this->updateCameraVectors();
	}

	// Destructors
	~Camera() {

	}

	// Update
	void updateInput(const float& delta, const int direction, const double& offsetX, const double& offsetY) {
		this->updateMouseInput(delta, offsetX, offsetY);
		this->updateKeyboardInput(delta, direction);
	}

	void updateMouseInput(const float& delta, const double& offsetX, const double& offsetY) {
		this->pitch += static_cast<GLfloat>(offsetY) * this->sensivity * delta;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensivity * delta;
		if (this->pitch > 75.f) {
			this->pitch = 75.f;
		}
		else if (this->pitch < -75.f){
			this->pitch = -75.f;
		}

		if (this->yaw > 360.f || this->yaw < -360.f) {
			this->yaw = 0.f;
		}
	}

	void updateKeyboardInput(const float& delta, const int direction) {
		switch (direction) {
		case 0:	//FORWARD
			this->position += this->front * this->movementSpeed * delta;
			break;
		case 1:	//BACKWARD
			this->position -= this->front * this->movementSpeed * delta;
			break;
		case 2: //RIGHT
			this->position += this->right * this->movementSpeed * delta;
			break;
		case 3: //LEFT
			this->position -= this->right * this->movementSpeed * delta;
			break;
		case 4: //INVERT Y
			this->invertX *= -1;
			break;
		case 5: //INVERT X
			this->invertY *= -1;
			break;
		case 6: //UP
			this->position += this->worldUp * this->movementSpeed * delta;
			break; // DOWN
		case 7:
			this->position -= this->worldUp * this->movementSpeed * delta;
			break;
		default:
			break;
		}
	}

	// Setters
	void setCameraPosition(glm::vec3 position) {
		this->position = position;
	}

	void resetFront() {
		this->pitch = 0.f;
		this->yaw = -90.f;
		this->roll = 0.f;
	}

	// Getters
	const glm::mat4 getViewMatrix() {
		this->updateCameraVectors();
		this->ViewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
		return this->ViewMatrix;
	}

	const glm::vec3 getPosition() {
		return this->position;
	}

	const glm::vec3 getFront() {
		return this->front;
	}
};