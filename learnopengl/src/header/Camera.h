#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera // the constructor parameter are cameraPos, lookAtPoint, worldUp, be careful!
{
public:
	enum class CameraMovement{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
public:
	Camera(glm::vec3 cameraPos, glm::vec3 lookAtPoint, glm::vec3 worldUp); 
	void cameraMovement(CameraMovement movement, float deltaTime);
	void cameraMouseMovement(float mouseXOffset, float mouseYOffset);
	void cameraMouseScroll(float yoffset);
	void cameraEulaTransform(float pitch, float yaw);
	glm::mat4 getViewMatrix();

	float getFov() {
		return m_fov;
	}
	glm::vec3 getCameraPosition() {
		return m_cameraPos;
	}
	glm::vec3 getCameraFront() {
		return m_cameraFront;
	}
private:
	float m_fov = 45.0f;
	glm::vec3 m_cameraPos;
	glm::vec3 m_worldUp;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;
	float m_pitch = 0.0f;
	float m_yaw = -135.0f;
	float m_cameraMoveSpeed = 3.0f;
	float m_cameraPitchSensitivity = 0.1f;
	float m_cameraYawSensitivity = 0.1f;
private:
	glm::mat4 lookAt(glm::vec3 cameraPos, glm::vec3 lookAtPoint, glm::vec3 worldUp);

};

