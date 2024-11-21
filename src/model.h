#ifndef __MODEL_H__
#define __MODEL_H__


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>


struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords; // vec2 for texture coordinates
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool loadFromObj(const std::string& filename);
    void render(GLuint shaderProgram);


    void setPostion(const glm::vec3& newPosition);
    void setRotaiton(const glm::vec3& newRotation);
    void setScale(const glm::vec3& newScale);


    const glm::mat4& getMatrixModel() const { return modelMatrix; }
    const glm::vec3& getPostion() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }

    void translate(const glm::vec3& offset);
    void rotate(const glm::vec3& angles);
    void scaleBy(const glm::vec3& factor);


private:
    void setupMesh();
    void updateModelMatrix();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;

    glm::mat4 modelMatrix;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};



class Model {
public:
    Model();
    ~Model();

    bool loadModel(const std::string& path);
    void render(GLuint shaderProgram);

    void setPostion(const glm::vec3 newPosition);
    void setRotation(const glm::vec3 newRotation);
    void setScale(const glm::vec3 newScale);

private:
    std::vector<Mesh> meshes;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif