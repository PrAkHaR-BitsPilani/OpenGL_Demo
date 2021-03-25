#include "Camera.h"

Camera::Camera(glm::vec3 position , glm::vec3 front , glm::vec3 up , float yaw , float pitch)
    : m_roll(0.0f) , m_cameraSpeed(SPEED) , m_cameraSensitivity(SENSITIVITY) , m_fov(FOV)
{
    m_cameraPos = position;
    m_cameraFront = front;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_cameraPos , m_cameraPos + m_cameraFront , m_cameraUp);
}

void Camera::processKeyboardInput(camera_movements direction , float deltatime)
{
    float velocity = m_cameraSpeed * deltatime;
    switch (direction)
    {
    case FORWARD:
        m_cameraPos += m_cameraFront * velocity;
        break;
    case BACKWARD:
        m_cameraPos -= m_cameraFront * velocity;
        break;
    case RIGHT:
        m_cameraPos += m_cameraRight * velocity;
        break;
    case LEFT:
        m_cameraPos -= m_cameraRight * velocity;
        break;
    default:
        break;
    }
}

void Camera::processMouseMovement(float x_offset , float y_offset , bool constrainedPitch)
{
    x_offset *= m_cameraSensitivity;
    y_offset *= m_cameraSensitivity;

    m_yaw += x_offset;
    m_pitch += y_offset;

    if(constrainedPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(float y_offset)
{
    m_fov -= y_offset;
     if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;
}

float Camera::getFOV()
{
    return m_fov;
}

glm::vec3 Camera::getPosition()
{
    return m_cameraPos;
}

void Camera::setSpeed(float speed)
{
    m_cameraSpeed = speed;
}

void Camera::recenter(glm::vec3 position)
{
    m_cameraPos = position;
    m_cameraFront = glm::vec3(0.0f , 0.0f , -1.0f);
    m_worldUp = glm::vec3(0.0f , 1.0f , 0.0f);
    m_yaw = YAW;
    m_pitch = PITCH;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    m_cameraFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront.y = sin(glm::radians(m_pitch));
    m_cameraFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_cameraRight = glm::cross(m_cameraFront , m_worldUp);
    m_cameraUp = glm::cross(m_cameraRight , m_cameraFront);

}