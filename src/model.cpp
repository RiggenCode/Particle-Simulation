#include "model.h"
#include <fstream>
#include <sstream> 
#include <iostream>
#include <algorithm>


Mesh::Mesh():  modelMatrix(1.0f), position(0.0f), rotation(0.0f), scale(1.0f) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Mesh::loadFromObj(const std::string& filename) {
    std::vector<glm::vec3> tempPositions;
    std::vector<glm::vec3> tempNormals; 
    std::vector<glm::vec2> tempTexCoords;

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            iss >> position.x >> position.y >> position.z;
            tempPositions.push_back(position);
        } else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (prefix == "vt") {
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        } else if (prefix == "f") {
            std::string vertex;
            for (int i = 0; i < 3; i++) {
                iss >> vertex;
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream viss(vertex);

                unsigned int vertexIndex, uvIndex, normalIndex;
                viss >> vertexIndex >> uvIndex >> normalIndex;

                if (vertexIndex == 0 || vertexIndex > tempPositions.size() ||
                    normalIndex == 0 || normalIndex > tempNormals.size() ||
                    uvIndex == 0 || uvIndex > tempTexCoords.size()) {
                    std::cerr << "Error: Index out of bounds in OBJ file: "
                              << "vertexIndex: " << vertexIndex << ", "
                              << "uvIndex: " << uvIndex << ", "
                              << "normalIndex: " << normalIndex << std::endl;
                    return false;
                }

                Vertex v;
                v.position = tempPositions[vertexIndex - 1];
                v.normal = tempNormals[normalIndex - 1];
                v.texCoords = tempTexCoords[uvIndex - 1];

                vertices.push_back(v);
                indices.push_back(vertices.size() - 1);
            }
        }
    }
    setupMesh();
    return true;
}

void Mesh::setupMesh() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // TexCoord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::render(GLuint shaderProgram) {
    updateModelMatrix();

    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setPostion(const glm::vec3& newPosition) {
    position = newPosition;
    updateModelMatrix();
}

void Mesh::setRotaiton(const glm::vec3& newRotation) {
    rotation = newRotation;
    updateModelMatrix();
}

void Mesh::setScale(const glm::vec3& newScale) {
    scale = newScale;
    updateModelMatrix();
}

void Mesh::translate(const glm::vec3& offset) {
    position += offset;
    updateModelMatrix();
}

void Mesh::rotate(const glm::vec3& angles) {
    rotation += angles;
    updateModelMatrix();
}

void Mesh::scaleBy(const glm::vec3& factor) {
    scale *= factor;
    updateModelMatrix();
}

void Mesh::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);
}

Model::Model() :
    position(0.0f),
    rotation(0.0f),
    scale(1.0f) {
}

Model::~Model() {
    meshes.clear();
}

bool Model::loadModel(const std::string& path) {
    Mesh mesh;
    if (!mesh.loadFromObj(path)) {
        return false;
    }
    meshes.push_back(mesh);
    return true;
}

void Model::render(GLuint shaderProgram) {
    for (Mesh& mesh : meshes) {
        mesh.render(shaderProgram);
    }
}

void Model::setPostion(const glm::vec3 newPosition) {
    position = newPosition;
    for (Mesh& mesh : meshes) {
        mesh.setPostion(position);
    }
}

void Model::setRotation(const glm::vec3 newRotation) {
    rotation = newRotation;
    for (Mesh& mesh : meshes) {
        mesh.setRotaiton(rotation);
    }
}

void Model::setScale(const glm::vec3 newScale) {
    scale = newScale;
    for (Mesh& mesh : meshes) {
        mesh.setScale(scale);
    }
}
