#include "Shader.hpp"

spry::Shader::Shader(const char* vertShaderSource, const char* fragShaderSource)
    : mVertShaderSource(vertShaderSource)
    , mFragShaderSource(fragShaderSource)
    , mHasCompiled(false)
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

    vertFile.open(mVertShaderSource);
    fragFile.open(mFragShaderSource);

    std::stringstream vertStream;
    std::stringstream fragStream;
    vertStream << vertFile.rdbuf();
    fragStream << fragFile.rdbuf();

    std::string vertCode = vertStream.str();
    std::string fragCode = fragStream.str();

    const char* vertCodeStr = vertCode.c_str();
    const char* fragCodeStr = fragCode.c_str();

    const auto getError = [&](unsigned int shader, bool is_vert_shader = true, bool is_program = false) {
        int success;
        char log[512];
        if (is_program) {
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
        } else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        }

        if (!success) {
            if (is_program) {
                glGetProgramInfoLog(ID, 512, nullptr, log);
                std::cout << "[ERROR]::"
                          << "LINK"
                          << "::\n";
            } else {
                glGetShaderInfoLog(shader, 512, nullptr, log);
                std::cout << "[ERROR]::" << (is_vert_shader ? "VERT" : "FRAG") << "::\n";
            }
            std::cout << log << std::endl;
        }
    };

    unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertCodeStr, nullptr);
    glCompileShader(vertShader);
    getError(vertShader);

    unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragCodeStr, nullptr);
    glCompileShader(fragShader);
    getError(fragShader, false);

    ID = glCreateProgram();
    glAttachShader(ID, vertShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    getError(ID, false, true);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    check_for_opengl_error();
}

void spry::Shader::use()
{
    glUseProgram(ID);
}

void spry::Shader::setUniformMatrix(const char* name, spry::Mat4<float>& value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, value.getData());
}

void spry::Shader::setUniformFloat(const char* name, float value)
{
    int loc = glGetUniformLocation(ID, name);
    glUniform1f(loc, value);
    // glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
