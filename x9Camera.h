#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//Defines several possible options for camera movement.
//Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default camera values
const float YAW			= -90.0f;
const float PITCH	    =    .0f;
const float SPEED		=   2.5f;
const float SENSITIVILITY =  .1f;
const float ZOOM		=  90.0f;

//An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors, Matrices for use in OpenGL.
class Camera
{
public:
	//camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//euler Angles
	float Yaw;
	float Pitch;
	//camera options
	float MovementSpeed;
	float MouseSensitivility;
	float Zoom;

	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(.0f, .0f, .0f), glm::vec3 up = glm::vec3(.0f, 1.0f, .0f),
		   float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(.0f, .0f, -1.0f)),
		   MovementSpeed(SPEED), MouseSensitivility(SENSITIVILITY), Zoom(ZOOM) 
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	//constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(.0f, .0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivility(SENSITIVILITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	//returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}


	//process keyboard input
	void processKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD) Position += Front * velocity;
		if (direction == BACKWARD)Position -= Front * velocity;
		if (direction == LEFT) Position -= Right * velocity;
		if (direction == RIGHT) Position += Right * velocity;
	}


	//processes mouse input. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= MouseSensitivility;
		yOffset *= MouseSensitivility;

		Yaw += xOffset;
		Pitch += yOffset;

		//make sure that when pitch is out of bouds, screen doesnt't get flipped
		if (constrainPitch) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		//update Front, Right, Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	//processes mouse scroll-wheel(vertical) event 
	void ProcessMouseScroll(float yOffset) {
		Zoom -= (float)yOffset;
		if (Zoom < 1.0f) Zoom = 1.0f;
		if (Zoom > 45.0f) Zoom = 45.0f;
	}






private:
	//calculates the front vector from the Camera's updated Euler Angles
	void updateCameraVectors() {
		//calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		//also re-calculate the Right and Up vector
		//length gets closer to 0 the more you look up or down which results in slower movement->normalize
		Right = glm::normalize(glm::cross(Front, WorldUp)); 
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif // !CAMERA_H

