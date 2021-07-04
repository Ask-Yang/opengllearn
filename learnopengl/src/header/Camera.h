#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
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
	void cameraEulaTransform(float pitch, float yaw);
	glm::mat4 getViewMatrix() {
		return lookAt(m_cameraPos, m_cameraFront + m_cameraPos, m_cameraUp);
	}
	float getFov() {
		return m_fov;
	}
	void cameraMouseScroll(float yoffset) {
		m_fov -= (float)yoffset;
		if (m_fov < 1.0f)
			m_fov = 1.0f;
		if (m_fov > 45.0f)
			m_fov = 45.0f;
	}
private:
	float m_fov = 30.0f;
	glm::vec3 m_cameraPos;
	glm::vec3 m_worldUp;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;
	float m_pitch = 0.0f;
	float m_yaw = -90.0f;
	float m_cameraMoveSpeed = 1.0f;
	float m_cameraPitchSensitivity = 0.1f;
	float m_cameraYawSensitivity = 0.1f;
private:
	glm::mat4 lookAt(glm::vec3 cameraPos, glm::vec3 lookAtPoint, glm::vec3 worldUp);

};

