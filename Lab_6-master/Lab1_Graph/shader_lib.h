#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class ShaderProgram {
public:
    GLuint ID;

    ShaderProgram(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        GLuint vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
        GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    ShaderProgram(ShaderProgram&& other) noexcept : ID(other.ID) {
        other.ID = 0;
    }

    ~ShaderProgram() {
        if (ID) glDeleteProgram(ID);
    }

    void use() const {
        glUseProgram(ID);
    }

    void setMat3(const std::string& name, const glm::mat3& mat) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
        }
    }

    void setVec3(const std::string& name, const glm::vec3& value) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1) {
            glUniform3fv(loc, 1, glm::value_ptr(value));
        }
    }

    void setFloat(const std::string& name, float value) const {
        GLint loc = getUniformLocation(name);
        if (loc != -1) {
            glUniform1f(loc, value);
        }
    }

private:
    GLint getUniformLocation(const std::string& name) const {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        if (loc == -1) {
            std::cerr << "Warning: Uniform '" << name << "' not found in shader!" << std::endl;
        }
        return loc;
    }

    std::string readFile(const char* path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << path << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLuint compileShader(GLenum type, const char* code) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        return shader;
    }

    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "Shader compilation error (" << type << "):\n" << infoLog << std::endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "Program linking error:\n" << infoLog << std::endl;
            }
        }
    }
};

#endif