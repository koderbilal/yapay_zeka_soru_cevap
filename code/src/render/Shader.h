#pragma once

#include "render/GLApi.h"

#include <glm/mat4x4.hpp>

#include <set>
#include <string>

namespace voxel {

// Compiled and linked GLSL program. Requires a current GL context and a loaded GLApi.
class Shader {
public:
    explicit Shader(GLApi& gl);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // Compiles both stages from files (via ReadFile). On failure returns false and
    // fills `error` with the file or driver log message.
    bool Load(const std::string& vertexPath,
              const std::string& fragmentPath,
              std::string& error);
    // Makes this the active program.
    void Use() const;
    // Sets a mat4 uniform; a missing uniform is reported once on stderr, then ignored.
    void SetMat4(const char* name, const glm::mat4& value) const;

private:
    GLuint Compile(GLenum type, const std::string& source, std::string& error);

    GLApi& gl_;
    GLuint program_ = 0;
    mutable std::set<std::string> missingUniforms_;
};

}
