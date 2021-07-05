#include "Camera.h"
using namespace glm;
Camera::Camera(vec3 cameraPos,vec3 lookAtPoint, vec3 worldUp)
	:m_cameraPos(cameraPos),
	m_worldUp(worldUp),
	m_cameraFront(0,0,0),
	m_cameraUp(0,0,0),
	m_cameraRight(0,0,0)
{
	vec3 camera_z = normalize(cameraPos - lookAtPoint);
	vec3 camera_x = normalize(cross(worldUp, camera_z));
	vec3 camera_y = normalize(cross(camera_z, camera_x));
	m_cameraFront = -camera_z;
	m_cameraUp = camera_y;
	m_cameraRight = camera_x;
}

void Camera::cameraMovement(CameraMovement movement, float deltaTime)
{
	if (movement == CameraMovement::FORWARD)
		m_cameraPos += deltaTime * m_cameraMoveSpeed * m_cameraFront;
	else if (movement == CameraMovement::BACKWARD)
		m_cameraPos -= deltaTime * m_cameraMoveSpeed * m_cameraFront;
	else if (movement == CameraMovement::LEFT)
		m_cameraPos -= deltaTime * m_cameraMoveSpeed * m_cameraRight;
	else if (movement == CameraMovement::RIGHT)
		m_cameraPos += deltaTime * m_cameraMoveSpeed * m_cameraRight;
}

void Camera::cameraMouseMovement(float mouseXOffset, float mouseYOffset)
{
	m_pitch += mouseYOffset * m_cameraPitchSensitivity;
	m_yaw += mouseXOffset * m_cameraYawSensitivity;
 	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;
	cameraEulaTransform(m_pitch, m_yaw);
}

void Camera::cameraEulaTransform(float pitch, float yaw)
{
	m_pitch = pitch;
	m_yaw = yaw;
	m_cameraFront.y = sin(radians(m_pitch));
	m_cameraFront.x = cos(radians(m_pitch)) * cos(radians(m_yaw));
	m_cameraFront.z = cos(radians(m_pitch)) * sin(radians(m_yaw));
	m_cameraFront = normalize(m_cameraFront);
	m_cameraRight = normalize(cross(m_worldUp , -m_cameraFront));
	m_cameraUp = normalize(cross(-m_cameraFront, m_cameraRight));
}

glm::mat4 Camera::getViewMatrix()
{
	return lookAt(m_cameraPos, m_cameraFront + m_cameraPos, m_cameraUp);
}

void Camera::cameraMouseScroll(float yoffset)
{
	m_fov -= (float)yoffset;
	if (m_fov < 1.0f)
		m_fov = 1.0f;
	if (m_fov > 45.0f)
		m_fov = 45.0f;
}

glm::mat4 Camera::lookAt(vec3 cameraPos, vec3 lookAtPoint, vec3 worldUp)
{
	vec3 camera_z = normalize(cameraPos - lookAtPoint);
	vec3 camera_x = normalize(cross(worldUp, camera_z));
	vec3 camera_y = normalize(cross(camera_z, camera_x));
	return transpose(mat4(vec4(camera_x, 0), vec4(camera_y, 0), vec4(camera_z, 0), vec4(0, 0, 0, 1))) *
		mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(-cameraPos, 1));
}
