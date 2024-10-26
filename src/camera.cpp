#include "camera.h"



Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(50.0f)
{
    Position = position;
    globalUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera:: GetViewMatrix() {
    return glm::lookAt(Position,Position + Front, Up);
}

void Camera::ProcessKeyboard(float deltaTime, Camera_Movement direction) {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }

     if (direction == BACKWARD) {
        Position -= Front * velocity;
    }

     if (direction == LEFT) {
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    }

     if (direction == RIGHT) {
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    }
}


void Camera::ProcessMouse(float xOffset, float yOffset) {

    // Multiply offset values by mouse sensitivity 
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;
    if (Pitch > 89.9f) {
        Pitch = 89.9f;
    } 
    if (Pitch < -89.9f) {
        Pitch = -89.9f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw) * cos(glm::radians(Pitch)));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, globalUp));
    Up = glm::normalize(glm::cross(Right, Front));
}



