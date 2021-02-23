#pragma once

#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

enum camera_movements{
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};
const float YAW = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float FOV        =  45.0f;

class Camera
{
private:
    //camera attributes
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;
    glm::vec3 m_worldUp;
    //euler angles
    float m_yaw;
    float m_pitch;
    float m_roll;
    //camera options
    float m_cameraSpeed;
    float m_cameraSensitivity;
    float m_fov; 

    void updateCameraVectors();

    
public:
    Camera(glm::vec3 position = glm::vec3(0.0f , 0.0f, 0.0f) , glm::vec3 front = glm::vec3(0.0f , 0.0f , -1.0f) , glm::vec3 up = glm::vec3(0.0f , 1.0f , 0.0f) , float yaw = YAW , float pitch = PITCH);

    glm::mat4 getViewMatrix();
    void processKeyboardInput(camera_movements direction , float deltatime);
    void processMouseMovement(float x_offset , float y_offset , bool constrainedPitch = true);
    void processMouseScroll(float y_offset);
    float getFOV();

};





