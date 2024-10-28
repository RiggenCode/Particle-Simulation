#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
    : m_position(position)
    , m_globalUp(up)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_movementSpeed(2.5f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(float deltaTime, Camera_Movement direction) {
    float velocity = m_movementSpeed * deltaTime;
    
    switch(direction) {
        case Camera_Movement::FORWARD:
            m_position += m_front * velocity;
            break;
        case Camera_Movement::BACKWARD:
            m_position -= m_front * velocity;
            break;
        case Camera_Movement::LEFT:
            m_position -= m_right * velocity;
            break;
        case Camera_Movement::RIGHT:
            m_position += m_right * velocity;
            break;
        case Camera_Movement::UP:
            m_position += m_up * velocity;
            break;
    }
}

void Camera::ProcessMouse(float xOffset, float yOffset) {
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // Constrain pitch
    if(m_pitch > 89.0f)
        m_pitch = 89.0f;
    if(m_pitch < -89.0f)
        m_pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_globalUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}