//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void OpenGL::Camera::invalidate_cache() noexcept {
    front = std::nullopt;
    right = std::nullopt;
    up = std::nullopt;
}


glm::vec3 OpenGL::Camera::getFront() const noexcept {
    if (!front.has_value()) {
        front = glm::vec3 {
            std::cos(getYaw()) * std::cos(getPitch()),
            std::sin(getPitch()),
            std::sin(getYaw()) * std::cos(getPitch())
        };
    }
    return front.value();
}

glm::vec3 OpenGL::Camera::getRight() const noexcept {
    if (!right.has_value()){
        right = glm::normalize(glm::cross(getFront(), world_up));
    }
    return right.value();
}

glm::vec3 OpenGL::Camera::getUp() const noexcept {
    if (!up.has_value()){
        up = glm::normalize(glm::cross(getRight(), getFront()));
    }
    return up.value();
}

glm::vec3 OpenGL::Camera::getPosition() const noexcept {
    return target - distance * getFront();
}

glm::mat4 OpenGL::Camera::getView() const noexcept {
    return glm::lookAt(getPosition(), target, world_up);
}

glm::mat4 OpenGL::Camera::getProjection(float aspect_ratio) const noexcept {
    return glm::perspective(fov, aspect_ratio, min_distance, max_distance);
}

float OpenGL::Camera::getPitch() const noexcept {
    return pitch;
}

void OpenGL::Camera::addPitch(float amount) noexcept {
    static const float pitch_min = std::nextafter(-glm::half_pi<float>(), std::numeric_limits<float>::max()), /* smallest x s.t. x > -π/2 */
                       pitch_max = std::nextafter(glm::half_pi<float>(), std::numeric_limits<float>::min()); /* biggest x s.t. x < π/2 */

    pitch = glm::clamp(pitch + amount, pitch_min, pitch_max);
    invalidate_cache();
}

float OpenGL::Camera::getYaw() const noexcept {
    return yaw;
}

void OpenGL::Camera::addYaw(float amount) noexcept {
    // The result should be always positive whether (yaw + amount) is positive or negative.
    // https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    constexpr auto two_pi = glm::two_pi<float>();
    yaw = glm::mod(glm::mod(yaw + amount, two_pi) + two_pi, two_pi);
    invalidate_cache();
}