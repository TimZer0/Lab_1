#ifndef SHADER_LIB_H
#define SHADER_LIB_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class ShaderProgram {
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexCode = readFile(vertexPath);
        std::string fragmentCode = readFile(fragmentPath);

        GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        checkCompileErrors(vertexShader, "VERTEX");
        checkCompileErrors(fragmentShader, "FRAGMENT");
        checkLinkErrors();

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    ~ShaderProgram() 
    {
        glDeleteProgram(programID);
    }

    void use() const 
    {
        glUseProgram(programID);
    }
    void setUniform(const std::string& name, float v0, float v1, float v2, float v3) const 
    {
        glUniform4f(glGetUniformLocation(programID, name.c_str()), v0, v1, v2, v3);
    }
    void setUniform(const std::string& name, float value) const 
    {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }

private:
    GLuint programID;

    std::string readFile(const std::string& filePath) const 
    {
        std::ifstream file(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    GLuint compileShader(const char* shaderCode, GLenum shaderType) const 
    {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
        return shader;
    }
    void checkCompileErrors(GLuint shader, const std::string& type) const 
    {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    void checkLinkErrors() const 
    {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programID, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
        }
    }
};

#endif