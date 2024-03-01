#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

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
    void set_uniform_float(const char* name, float value);
    void set_uniform_matrix(const char *name, glm::mat4& value);
    void set_uniform_vec(const char *name, glm::vec4&& value);
    void set_uniform_vec(const char *name, glm::vec3&& value);

    void set_uniform_vec(const char *name, glm::vec4& value);
    void set_uniform_vec(const char *name, glm::vec3& value);

    void set_uniform_int(const char *name, int value) const;
};
}