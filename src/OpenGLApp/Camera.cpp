//
// Created by gomkyung2 on 2023/08/29.
//

#include "OpenGLApp/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void OpenGL::CameraView::invalidate_cache() noexcept {
    front = std::nullopt;
    right = std::nullopt;
    up = std::nullopt;
}


glm::vec3 OpenGL::CameraView::getFront() const noexcept {
    if (!front.has_value()) {
        front = glm::vec3 {
            std::sin(getYaw()) * std::cos(getPitch()),
            std::sin(getPitch()),
            -std::cos(getYaw()) * std::cos(getPitch())
        };
    }
    return front.value();
}

glm::vec3 OpenGL::CameraView::getRight() const noexcept {
    if (!right.has_value()){
        right = glm::normalize(glm::cross(getFront(), world_up));
    }
    return right.value();
}

glm::vec3 OpenGL::CameraView::getUp() const noexcept {
    if (!up.has_value()){
        up = glm::normalize(glm::cross(getRight(), getFront()));
    }
    return up.value();
}

glm::vec3 OpenGL::CameraView::getPosition() const noexcept {
    return target - distance * getFront();
}

glm::mat4 OpenGL::CameraView::getMatrix() const noexcept {
    return glm::lookAt(getPosition(), target, world_up);
}

float OpenGL::CameraView::getPitch() const noexcept {
    return pitch;
}

void OpenGL::CameraView::addPitch(float amount) noexcept {
    static const float pitch_min = std::nextafter(-glm::half_pi<float>(), std::numeric_limits<float>::max()), /* smallest x s.t. x > -π/2 */
                       pitch_max = std::nextafter(glm::half_pi<float>(), std::numeric_limits<float>::min()); /* biggest x s.t. x < π/2 */

    pitch = glm::clamp(pitch + amount, pitch_min, pitch_max);
    invalidate_cache();
}

float OpenGL::CameraView::getYaw() const noexcept {
    return yaw;
}

void OpenGL::CameraView::addYaw(float amount) noexcept {
    // The result should be always positive whether (yaw + amount) is positive or negative.
    // https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    constexpr auto two_pi = glm::two_pi<float>();
    yaw = glm::mod(glm::mod(yaw + amount, two_pi) + two_pi, two_pi);
    invalidate_cache();
}

glm::mat4 OpenGL::PerspectiveProjection::getMatrix(float region_aspect_ratio) const noexcept {
    return glm::perspective(fov, region_aspect_ratio, near_distance, far_distance);
}

glm::mat4 OpenGL::OrthographicProjection::getMatrix(glm::vec2 region_position, glm::vec2 region_size) const noexcept {
    return glm::ortho(region_position.x, region_position.x + region_size.x, region_position.y, region_position.y + region_size.y, near_distance, far_distance);
}