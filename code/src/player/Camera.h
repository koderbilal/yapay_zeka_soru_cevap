#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace voxel {

enum class CameraMove {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera {
public:
    glm::vec3 Position{0.0f, 0.0f, 3.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float Speed = 4.5f;

    glm::mat4 GetViewMatrix() const;
    void Move(CameraMove direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);

private:
    void UpdateFront();
};

}
