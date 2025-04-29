#ifndef SHADER_LIB_H
#define SHADER_LIB_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class ShaderProgram {
public:
    GLuint programID;

    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexCode = loadFile(vertexPath);
        std::string fragmentCode = loadFile(fragmentPath);

        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        checkShaderCompile(vertexShader, "VERTEX");
        checkShaderCompile(fragmentShader, "FRAGMENT");
        checkProgramLink();

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
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

    void setMat4(const std::string& name, const float* value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
    }

private:
    std::string loadFile(const std::string& path) const
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Could not open file: " << path << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    GLuint compileShader(GLenum type, const char* code) const
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &code, nullptr);
        glCompileShader(shader);
        return shader;
    }

    void checkShaderCompile(GLuint shader, const std::string& type) const
    {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader Compilation Error (" << type << "): " << infoLog << std::endl;
        }
    }

    void checkProgramLink() const
    {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
            std::cerr << "Program Linking Error: " << infoLog << std::endl;
        }
    }
};

#endif