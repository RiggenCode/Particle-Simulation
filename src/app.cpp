#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include <string>
#include <sstream>

int screenWidth = 800;
int screenHeight = 600;


 void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    }

void processInput(GLFWwindow* window , Camera& camera, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(deltaTime, LEFT);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(deltaTime, BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(deltaTime, RIGHT);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(deltaTime, FORWARD);
    }


}


void renderScene(Camera& camera, int width, int height) {
    glm::mat4 view = camera.GetViewMatrix();  // Get the updated view matrix from the camera
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / height, 0.1f, 100.0f);
    // Render particles and other objects here
}




int main() {

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif


    // Create window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Window Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to Initialize GLEW" << std::endl;
        return -1;
    }


    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);


    int frameCount = 0;
    float lastFrame = 0.0f;
    float fpsTimer = 0.0f;

    // Render loop
    while(!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frameCount++;
        fpsTimer += deltaTime;

        if (fpsTimer >= 1.0f) {
            int fps = frameCount;

            frameCount = 0;
            fpsTimer = 0.0f;
            std::stringstream fpsString;
            fpsString << "FPS: " << fps;
            glfwSetWindowTitle(window, fpsString.str().c_str());
        }

        processInput(window, camera, deltaTime);

        glClearColor((float)45/256, (float)45/256, (float)45/256, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderScene(camera, screenWidth, screenHeight);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    

    glfwTerminate();
    return 0;
}


