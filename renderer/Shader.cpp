#include "Shader.hpp"

spry::Shader::Shader(const char* vertShaderSource, const char* fragShaderSource)
    : mVert_shader_path(vertShaderSource)
    , mFrag_shader_path(fragShaderSource)
    , mHasCompiled(false)
{
}

spry::Shader::Shader()
{
}

spry::Shader::~Shader()
{
    glDeleteProgram(ID);
}

void spry::Shader::compile()
{
    if (mHasCompiled) {
        return;
    }

    mHasCompiled = true;

    std::ifstream vertFile;
    std::ifstream fragFile;

    vertFile.open(mVert_shader_path);
    fragFile.open(mFrag_shader_path);

    std::stringstream vertStream;
    std::stringstream fragStream;
    vertStream << vertFile.rdbuf();
    fragStream << fragFile.rdbuf();

    std::string vertCode = vertStream.str();
    std::string fragCode = fragStream.str();

    const char* vertCodeStr = vertCode.c_str();
    const char* fragCodeStr = fragCode.c_str();

    compile_shader_code(vertCodeStr, fragCodeStr);

    check_for_opengl_error();
}

void spry::Shader::compile_shader_code(const char* vertex, const char* fragment)
{
    const auto getError = [&](unsigned int shader, bool is_vert_shader = true, bool is_program = false) {
        int success;
        char log[1024];
        if (is_program) {
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
        } else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        }

        if (!success) {
            if (is_program) {
                glGetProgramInfoLog(ID, 1024, nullptr, log);
                std::cout << "[ERROR]::LINK::";
            } else {
                glGetShaderInfoLog(shader, 1024, nullptr, log);
                std::cout << "[ERROR]::" << (is_vert_shader ? "VERT" : "FRAG") << "::" << mVert_shader_path << "::";
            }
            std::cout << log << std::endl;
        }
    };

    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertex, nullptr);
    glCompileShader(vertShader);
    getError(vertShader);

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragment, nullptr);
    glCompileShader(fragShader);
    getError(fragShader, false);

    ID = glCreateProgram();
    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    getError(ID, false, true);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

void spry::Shader::use()
{
    glUseProgram(ID);
}

void spry::Shader::set_uniform_float(const char* name, float value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform1f(loc, value);
}

void spry::Shader::set_uniform_matrix(const char* name, glm::mat4& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void spry::Shader::set_uniform_vec(const char* name, glm::vec3&& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}


void spry::Shader::set_uniform_vec(const char* name, glm::vec4&& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_uniform_vec(const char* name, glm::vec3& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_uniform_int(const char* name, int value) const
{
    int loc = glGetUniformLocation(ID, name);
    glUniform1i(loc, value);
}

void spry::Shader::set_uniform_vec(const char* name, glm::vec4& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void spry::Shader::set_shader_paths(const char* vert_shader_path, const char* frag_shader_path)
{
    mVert_shader_path = vert_shader_path;
    mFrag_shader_path = frag_shader_path;
    mHasCompiled = false;
}

void spry::Shader::set_shader_code(const char* vert_shader_source, const char* frag_shader_source)
{
    compile_shader_code(vert_shader_source, frag_shader_source);
    mHasCompiled = true;
}

spry::Shader spry::ShaderManager::simple_shader()
{
    Shader shader("", "");
    shader.mHasCompiled = true;

    const char* vert_shader = R"(
        #version 330
        layout (location = 0) in vec3 apos;

        void main() {
            gl_Position = vec4(apos, 1.0);
        }
    )";

    const char* frag_shader = R"(
        #version 330 core 

        out vec4 frag_color;
        in vec4 color;

        void main() {
            frag_color = color;
        }
    )";

    shader.compile_shader_code(vert_shader, frag_shader);
    return shader;
}

spry::Shader spry::ShaderManager::mvp_shader()
{
    Shader shader("", "");
    shader.mHasCompiled = true;

    const char* vert_shader = R"(
        #version 330
        layout (location = 0) in vec3 apos;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(apos, 1.0);
        }
    )";

    const char* frag_shader = R"(
        #version 330 core 

        out vec4 frag_color;
        
        uniform vec4 color;

        void main() {
            frag_color = color;
        }
    )";

    shader.compile_shader_code(vert_shader, frag_shader);
    return shader;
}

spry::Shader spry::ShaderManager::create_shader(const char* vert_code, const char* frag_code)
{
    Shader shader("", "");
    shader.mHasCompiled = true;
    shader.compile_shader_code(vert_code, frag_code);
    return shader;
}
