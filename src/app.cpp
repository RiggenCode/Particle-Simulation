#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "model.h"
#include "shader.h"
#include <iostream>
#include <string>
#include <sstream>



int screenWidth = 800;
int screenHeight = 600;

 void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Create camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);

void processInput(GLFWwindow* window, float deltaTime) {
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(deltaTime, UP);
    }


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 800.0f / 2.0f; // Assuming window width is 800
    static float lastY = 600.0f / 2.0f; // Assuming window height is 600
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // Invert y-axis for proper camera control
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouse(xOffset, yOffset); // Call your ProcessMouse function
}

void renderScene(Camera& camera, int width, int height, Shader& shader, Mesh* mesh) {
    glEnable( GL_DEPTH_TEST );

    glm::mat4 view = camera.GetViewMatrix();  // Get the updated view matrix from the camera
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)width / height, 0.1f, 100.0f);

    shader.use();

    // Set matrices
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // Set light properties
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.GetPosition());
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("objectColor", objectColor);

    // Render the mesh
    mesh->render(shader.getProgram());


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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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


    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to Initialize GLEW" << std::endl;
        return -1;
    }

    // Create and intialize shader
    Shader shader;
    if(!shader.loadFromFile("/home/riggen/code/particle_simulation/shaders/vertex.glsl", "/home/riggen/code/particle_simulation/shaders/fragment.glsl")) {
        std::cerr << "Failed to load shader" << std::endl;
        return -1;
    }

    // Create and initialize mesh
    Mesh* mesh = new Mesh();
    mesh->loadFromObj("/home/riggen/code/particle_simulation/models/sphere.obj");
    if(!mesh->loadFromObj("/home/riggen/code/particle_simulation/models/sphere.obj")) {
        std::cerr << "Failed to load mesh" << std::endl;
        return -1;
    }

    // Set the initial transform of the mesh
    mesh->setPostion(glm::vec3(0.0f, 0.0f, 0.0f));
    mesh->setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    // Setup viewport and callbacks
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // frame timing variables
    int frameCount = 0;
    double lastTime = glfwGetTime();
    float fpsTimer = 0.0f;

    // Render loop
    while(!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

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

        processInput(window, deltaTime);

        glClearColor((float)25.6/256, (float)25.6/256, (float)25.6/256, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderScene(camera, screenWidth, screenHeight, shader, mesh);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    
    delete mesh;
    glfwTerminate();
    return 0;
}


