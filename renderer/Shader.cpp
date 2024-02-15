#include "Shader.hpp"

Shader::~Shader() {
    glDeleteProgram(ID);
}

void Shader::compile(const char *vert_source, const char *frag_source) {
    std::ifstream vert_file;
    std::ifstream frag_file;

    vert_file.open(vert_source);
    frag_file.open(frag_source);

    std::stringstream vert_stream;
    std::stringstream frag_stream;
    vert_stream << vert_file.rdbuf();
    frag_stream << frag_file.rdbuf();

    std::string vert_code = vert_stream.str();
    std::string frag_code = frag_stream.str();

    const char *vert_code_str = vert_code.c_str();
    const char *frag_code_str = frag_code.c_str();

    const auto get_error = [&](unsigned int shader, bool is_vert_shader = true, bool is_program = false) {
        int success;
        char log[512];
        if (is_program) {
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
        }
        else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        }

        if (!success) {
            if (is_program) {
                glGetProgramInfoLog(ID, 512, nullptr, log);
                std::cout << "[ERROR]::" << "LINK" << "::\n";
            }
            else {
                glGetShaderInfoLog(shader, 512, nullptr, log);
                std::cout << "[ERROR]::" << (is_vert_shader ? "VERT" : "FRAG") << "::\n";
            }
            std::cout << log << std::endl;
        }

    };

    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_code_str, nullptr);
    glCompileShader(vert_shader);
    get_error(vert_shader);

    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_code_str, nullptr);
    glCompileShader(frag_shader);
    get_error(frag_shader, false);

    ID = glCreateProgram();
    glAttachShader(ID, vert_shader);
    glAttachShader(ID, frag_shader);
    glLinkProgram(ID);
    get_error(ID, false, true);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    check_for_opengl_error();
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::set_uniform_matrix(const char *name, glm::mat4& value) {
    int loc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform_float(const char *name, float value) {
    int loc = glGetUniformLocation(ID, name);
    glUniform1f(loc, value);
    // glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
