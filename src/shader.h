#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromString(const char* vertexSource, const char* fragmentSource);
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    GLuint getProgram() const { return program; }

    // Utility functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
private:
    bool compileAndLink(const char* vertexSource, const char* fragmentSource);
    bool checkCompileErrors(GLuint shader, const std::string& type);

    GLuint program;
    GLuint vertexShader;
    GLuint fragmentShader;

};

#endif