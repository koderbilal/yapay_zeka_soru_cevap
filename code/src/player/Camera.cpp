#include "player/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>

namespace voxel {

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Move(CameraMove direction, float deltaTime) {
    const float distance = Speed * deltaTime;
    const glm::vec3 horizontalFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    const glm::vec3 right = glm::normalize(glm::cross(Front, Up));

    switch (direction) {
    case CameraMove::Forward:
        Position += horizontalFront * distance;
        break;
    case CameraMove::Backward:
        Position -= horizontalFront * distance;
        break;
    case CameraMove::Left:
        Position -= right * distance;
        break;
    case CameraMove::Right:
        Position += right * distance;
        break;
    case CameraMove::Up:
        Position += Up * distance;
        break;
    case CameraMove::Down:
        Position -= Up * distance;
        break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
    constexpr float sensitivity = 0.1f;
    Yaw += xOffset * sensitivity;
    Pitch = std::clamp(Pitch + yOffset * sensitivity, -89.0f, 89.0f);
    UpdateFront();
}

void Camera::UpdateFront() {
    const float yawRadians = glm::radians(Yaw);
    const float pitchRadians = glm::radians(Pitch);
    Front = glm::normalize(glm::vec3(
        std::cos(yawRadians) * std::cos(pitchRadians),
        std::sin(pitchRadians),
        std::sin(yawRadians) * std::cos(pitchRadians)
    ));
}

}
