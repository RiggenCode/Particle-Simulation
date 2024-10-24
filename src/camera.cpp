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

void Camera::ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right) {
    float velocity = MovementSpeed * deltaTime;
    if (forward) {
        Position += Front * velocity;
    }

     if (backward) {
        Position -= Front * velocity;
    }

     if (left) {
        Position -= glm::normalize(glm::cross(Front, Up)) * velocity;
    }

     if (right) {
        Position += glm::normalize(glm::cross(Front, Up)) * velocity;
    }

    void Camera::ProcessMouse(float xOffset, float yOffset) {

        // Multiply offset values by mouse sensitivity 
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;


    }
}


