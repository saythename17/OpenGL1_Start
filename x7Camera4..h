#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

///Defines several possible options for camera movement.
//Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement { //User-defined data type
	FORWARD, BACKWARD, LEFT, RIGHT
};

//Default camera values
const float YAW         = -90.0f;
/*to make sure the camera points towards the negative z-axis by default,
we can give the yaw a value of a 
90 degree clockwise rotation.-----(X)
-90 positive degrees rotate counter-clockwise*/
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.01f;
const float ZOOM        =  45.0f;

//An abstract camera class that processes input and calculates 
//the corresponding Euler Angles, Ventors and Matrices for use in OpenGL.
class Camera {
public:
	//Camera Attributes
	glm::vec3 Position, Front, Up, Right, WorldUp;
	//Euler Angles
	float Yaw, Pitch; 
	//Camera Options
	float MovementSpeed, MouseSensitivity, Zoom/*fov*/;
	/* fov : how much we can see of the scene
	*When the fov becomes smaller, the scene's projected space get smaller. It giving the illusion of zooming in.*/





	//constructor with vectors
	Camera(glm::vec3 position = glm::vec3(.0f, .0f, .0f), 
		   glm::vec3 up = glm::vec3(.0f, 1.0f, .0f),
		   float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(.0f, .0f, -1.0f)),
		   MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//constructor with scalar values
	Camera(float posX, float posY, float posZ,
		   float upX, float upY, float upZ,
		   float yaw, float pitch) : Front(glm::vec3(.0f, .0f, -1.0f)), // -z axis
		   MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
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
		/*Get the camera position
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	//z-axis is going through your screen towards you
	//so if you want to the camera to move backwards, we move along the positive z-aixs.
	//note the position vector is inverted since we want to move the world in opposite direction of where the camera to move.

	//Get the camera direction = camera position vector - scene's origin vector
	glm::vec3 cameraTarget = glm::vec3(.0f, .0f, .0f);// let the camera point to the origin of the screen
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	//└It is actually pointing in the reverse direction of what if is targeting

	//Get the right vector
	//└ that represents the positive x-axis of the camera space.
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//points upwards in world space
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//cross product up(+y-axis) × direction(+z-axis) = positive x-axis's direction
	// p.s)) direction × up = negative x-axis

	//Get the up axis = right(+x) × direction(+z)
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 view=glm::lookAt(glm::vec3(.0f, .0f,3.0f),//position
							   glm::vec3(.0f, .0f, .0f),//target
							   glm::vec3(.0f,1.0f, .0f));//up
	//LookAt matrix - transfroms all the world coordinates to the view space we defined.
	//				- makes view matrix looks at a given target
	/*To move a camera backwards == To move the entire scene forward
	* OpenGL is a right-handed syste. So if we want to move backwards,
	* we have to move in the positive z-axis. == translate the scene towards the negative z-axis.
	* (move the entire scene around inversed to where we want to the camera to move)
	* This gives the impression that we are moving backwards.*/
	}






	//processes input received from keyboard input. 
	//Accepts input parameter in the form of User-defined ENUM(to abstract it from window system)
	void ProcessKeyboardFPS(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)	Position += Front * velocity;
		if (direction == BACKWARD)  Position -= Front * velocity;
		if (direction == LEFT)		Position -= Right * velocity;
		if (direction == RIGHT)		Position += Right * velocity;

		//make sure the user stays at ground level
		Position.y = 0.0f; //<--this one-liner keeps the user at the ground(xz plane)
	}
	
	//processes input received from a mouse. offset value in both the x & y direction.
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;
		Yaw += xOffset;
		Pitch += yOffset;

		//make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (Pitch > 89.0f)Pitch = 89.0f;
			if (Pitch < -89.0f)Pitch = -89.0f;
		}

		//update Front, Right, Up vectors using the updated Euler angles
		updateCameraVectors();
	}
	//processes input received from a mouse scroll-wheel. input on the vertival wheel axis
	void ProcessMouseScroll(float yOffset) {
		Zoom -= (float)yOffset;
		if (Zoom < 1.0f)	Zoom = 0.0f;
		if (Zoom > 90.0f)	Zoom = 90.0f;
	}
	/*  Handling mouse input
	* 1.Calculate the mouse's offset since the last frame.
	* 2.Add the offset values to the camera's yaw and pitch values.
	* 3.Add some constraints to the minimum/maximum pitch values.
	* 4.Calculate the direction vector.*/







private:
	//calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors() {
		//calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		//also re-calculate the Right and Up vector
		//normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif
