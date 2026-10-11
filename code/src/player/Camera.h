#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

namespace voxel {

// Directions accepted by Camera::Move.
enum class CameraMove {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

// Free-fly camera. Yaw/Pitch are in degrees; Pitch is clamped to [-89, 89].
class Camera {
public:
    glm::vec3 Position{0.0f, 0.0f, 3.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float Speed = 4.5f;
    float Sensitivity = 0.1f;

    Camera();

    // View matrix looking from Position along Front.
    glm::mat4 GetViewMatrix() const;
    // Moves by Speed * deltaTime (seconds). Forward/Backward/Left/Right stay on
    // the horizontal plane; Up/Down move along the world Y axis.
    void Move(CameraMove direction, float deltaTime);
    // Applies a mouse delta in pixels (yOffset positive = look up).
    void ProcessMouseMovement(float xOffset, float yOffset);

private:
    void UpdateFront();
};

}
