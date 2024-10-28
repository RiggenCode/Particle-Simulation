#include "shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

    
Shader::Shader() : program(0) {}
Shader::~Shader() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

bool Shader::loadFromString(const char* vertexSource, const char* fragmentSource) {
    return compileAndLink(vertexSource, fragmentSource);
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    
    try {
        // Read vertex shader file
        std::ifstream vShaderFile(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();
        
        // Read fragment shader file
        std::ifstream fShaderFile(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        return false;
    }
    // std::cout << "Vertex Shader Code:\n" << vertexCode << "\n";
    // std::cout << "Fragment Shader Code:\n" << fragmentCode << "\n";
    return compileAndLink(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::use() {
    glUseProgram(program);
}

// Uniform setters
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}


bool Shader::compileAndLink(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Compile vertex shader
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    if (!checkCompileErrors(vertexShader, "VERTEX")) {
        glDeleteShader(vertexShader);
        return false;
    }
    
    // Compile fragment shader
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    if (!checkCompileErrors(fragmentShader, "FRAGMENT")) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }
    
    // Link shaders
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    if (!checkCompileErrors(program, "PROGRAM")) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        program = 0;
        return false;
    }
    
    // Delete shaders as they're linked into our program and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

bool Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                        << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            return false;
        }
    }
    return true;
}
