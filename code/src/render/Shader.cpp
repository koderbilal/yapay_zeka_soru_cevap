#include "render/Shader.h"

#include "core/File.h"

#include <vector>

namespace voxel {

Shader::Shader(GLApi& gl) : gl_(gl) {}

Shader::~Shader() {
    if (program_ != 0) {
        gl_.DeleteProgram(program_);
    }
}

GLuint Shader::Compile(GLenum type, const std::string& source, std::string& error) {
    const GLuint shader = gl_.CreateShader(type);
    const GLchar* sourceText = source.c_str();
    gl_.ShaderSource(shader, 1, &sourceText, nullptr);
    gl_.CompileShader(shader);

    GLint compiled = GL_FALSE;
    gl_.GetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return shader;
    }

    GLint logLength = 0;
    gl_.GetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> log(static_cast<std::size_t>(logLength > 0 ? logLength : 1));
    gl_.GetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
    error.assign(log.begin(), log.end());
    gl_.DeleteShader(shader);
    return 0;
}

bool Shader::Load(const std::string& vertexPath,
                 const std::string& fragmentPath,
                 std::string& error) {
    const auto vertexBytes = ReadFile(vertexPath);
    const auto fragmentBytes = ReadFile(fragmentPath);
    if (vertexBytes.empty() || fragmentBytes.empty()) {
        error = "Could not read shader files: " + vertexPath + " and " + fragmentPath;
        return false;
    }

    const std::string vertexSource(vertexBytes.begin(), vertexBytes.end());
    const std::string fragmentSource(fragmentBytes.begin(), fragmentBytes.end());
    const GLuint vertex = Compile(GL_VERTEX_SHADER, vertexSource, error);
    if (vertex == 0) {
        error = "Vertex shader compilation failed: " + error;
        return false;
    }

    const GLuint fragment = Compile(GL_FRAGMENT_SHADER, fragmentSource, error);
    if (fragment == 0) {
        gl_.DeleteShader(vertex);
        error = "Fragment shader compilation failed: " + error;
        return false;
    }

    const GLuint program = gl_.CreateProgram();
    gl_.AttachShader(program, vertex);
    gl_.AttachShader(program, fragment);
    gl_.LinkProgram(program);
    gl_.DeleteShader(vertex);
    gl_.DeleteShader(fragment);

    GLint linked = GL_FALSE;
    gl_.GetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        GLint logLength = 0;
        gl_.GetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> log(static_cast<std::size_t>(logLength > 0 ? logLength : 1));
        gl_.GetProgramInfoLog(program, static_cast<GLsizei>(log.size()), nullptr, log.data());
        error.assign(log.begin(), log.end());
        gl_.DeleteProgram(program);
        error = "Shader program linking failed: " + error;
        return false;
    }

    if (program_ != 0) {
        gl_.DeleteProgram(program_);
    }
    program_ = program;
    return true;
}

void Shader::Use() const {
    gl_.UseProgram(program_);
}

void Shader::SetMat4(const char* name, const glm::mat4& value) const {
    const GLint location = gl_.GetUniformLocation(program_, name);
    if (location >= 0) {
        gl_.UniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

}
