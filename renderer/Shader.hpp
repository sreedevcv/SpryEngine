#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mat4.hpp"
#include "utils.hpp"

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
    void setUniformMatrix(const char* name, spry::Mat4<float>& value);
    void setUniformFloat(const char* name, float value);
};
}