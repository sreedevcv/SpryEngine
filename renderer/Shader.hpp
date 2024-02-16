#pragma once 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "Mat4.hpp"


class Shader {
private:
    unsigned int ID;
public:
    Shader() = default;
    ~Shader();

    void compile(const char *vert_shader, const char *frag_shader);
    void use();
    void set_uniform_matrix(const char *name, spry::Mat4& value);
    void set_uniform_float(const char *name, float value);
};