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

class ShaderManager;

class Shader {
private:
    unsigned int ID;
    const char* mVert_shader_path;
    const char* mFrag_shader_path;
    bool mHasCompiled = false;

    void compile_shader_code(const char* vertex, const char* fragment);

public:
    Shader();
    Shader(const char* vert_shader_path, const char* fragShaderSource);
    ~Shader();

    void set_shader_paths(const char* vert_shader_path, const char* frag_shader_path);
    void set_shader_code(const char* vert_shader_source, const char* frag_shader_source);
    void compile();

    void use();
    void set_uniform_float(const char* name, const float value);
    void set_uniform_matrix(const char* name, const glm::mat4& value);
    void set_uniform_vec(const char* name, const glm::vec4&& value);
    void set_uniform_vec(const char* name, const glm::vec3&& value);
    void set_uniform_vec(const char* name, const glm::vec4& value);
    void set_uniform_vec(const char* name, const glm::vec3& value);
    void set_uniform_int(const char* name, const int value) const;

    friend class ShaderManager;
};

class ShaderManager {

public:
    static Shader simple_shader();
    static Shader mvp_shader();
    static Shader create_shader(const char* vert_code, const char* frag_code);
};
}