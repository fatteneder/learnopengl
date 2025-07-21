#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
  public:
    unsigned int ID; // program id

    Shader(const char *vertexPath, const char*fragmentPath)
    {
      // load source code
      std::string vertexCode;
      std::string fragmentCode;
      std::ifstream vShaderFile;
      std::ifstream fShaderFile;

      vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
      } catch (std::ifstream::failure e) {
        std::cout << "ERROR:SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      }
      const char *vShaderCode = vertexCode.c_str();
      const char *fShaderCode = fragmentCode.c_str();

      // compile shaders
      unsigned int vertex, fragment;
      int success;
      char infoLog[512];

      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
      if (!success) {
          glGetShaderInfoLog(vertex, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      }

      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
      if (!success) {
          glGetShaderInfoLog(fragment, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      }

      // link shaders into a program
      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);
      glGetProgramiv(ID, GL_LINK_STATUS, &success);
      if (!success) {
          glGetProgramInfoLog(ID, 512, NULL, infoLog);
          std::cout << "ERROR::SHADER::PROGARM::LINKING_FAILED\n" << infoLog << std::endl;
      }

      // cleanup shaders
      glDeleteShader(vertex);
      glDeleteShader(fragment);
    }

    void use() { glUseProgram(ID); }

    void setBool(const std::string &name, bool value) const {
      setInt(name, (int)value);
    }

    void setInt(const std::string &name, int value) const {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const std::string &name, glm::mat4 mat) const {
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setVec3(const std::string &name, glm::vec3 vec) const {
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
    }
};

#endif
