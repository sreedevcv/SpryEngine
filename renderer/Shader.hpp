#pragma once 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"


class Shader {
private:
    unsigned int ID;
public:
    Shader() = default;
    ~Shader();

    void compile(const char *vert_shader, const char *frag_shader);
    void use();
    void set_uniform_matrix(const char *name, glm::mat4& value);
    void set_uniform_float(const char *name, float value);
};