//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::vec3 OpenGL::Camera::getFront() const noexcept {
    return {
            std::cos(yaw) * std::cos(pitch),
            std::sin(pitch),
            std::sin(yaw) * std::cos(pitch)
    };
}

glm::mat4 OpenGL::Camera::getView() const noexcept {
    const glm::vec3 position = target - distance * getFront();
    return glm::lookAt(position, target, OpenGL::Camera::up);
}

glm::mat4 OpenGL::Camera::getProjection(float aspect_ratio) const noexcept {
return glm::perspective(fov, aspect_ratio, min_distance, max_distance);
}

void OpenGL::Camera::addPitch(float amount) noexcept {
    static const float pitch_min = std::nextafter(-glm::half_pi<float>(), std::numeric_limits<float>::max()), /* smallest x s.t. x > -π/2 */
    pitch_max = std::nextafter(glm::half_pi<float>(), std::numeric_limits<float>::min()); /* biggest x s.t. x < π/2 */

    pitch = glm::clamp(pitch + amount, pitch_min, pitch_max);
}

glm::vec3 OpenGL::Camera::getPosition() const noexcept {
    return target - distance * getFront();
}