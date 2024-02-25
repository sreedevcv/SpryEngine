#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

#include "Mat4.hpp"
#include "utils.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace spry {

class Shader {
private:
    unsigned int ID;
    const char* mVertShaderSource;
    const char* mFragShaderSource;
    bool mHasCompiled = false;

public:
    Shader(const char* vertShaderSource, const char* fragShaderSource);
    ~Shader();

    void compile();
    void use();
    void set_uniform_matrix(const char* name, spry::Mat4<float>& value);
    void setUniformFloat(const char* name, float value);

    void set_uniform_matrix(const char *name, glm::mat4& value);
};
}