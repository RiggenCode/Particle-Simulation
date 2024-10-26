#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    
    // Public methods
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(float deltaTime, Camera_Movement direction);
    void ProcessMouse(float xOffset, float yOffset);
    
    // Getters
    float GetZoom() const { return m_zoom; }
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetFront() const { return m_front; }

    // Setters (if needed)
    void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    void SetZoom(float zoom) { m_zoom = zoom; }

private:
    // Camera vectors
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_globalUp;

    // Euler angles
    float m_pitch;
    float m_yaw;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    // Private methods
    void updateCameraVectors();
};

#endif