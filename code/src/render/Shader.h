#pragma once

#include "render/GLApi.h"

#include <glm/mat4x4.hpp>

#include <set>
#include <string>

namespace voxel {

class Shader {
public:
    explicit Shader(GLApi& gl);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool Load(const std::string& vertexPath,
              const std::string& fragmentPath,
              std::string& error);
    void Use() const;
    void SetMat4(const char* name, const glm::mat4& value) const;

private:
    GLuint Compile(GLenum type, const std::string& source, std::string& error);

    GLApi& gl_;
    GLuint program_ = 0;
    mutable std::set<std::string> missingUniforms_;
};

}
