//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

#include <optional>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/constants.hpp>

namespace OpenGL{
    class CameraView{
    private:
        static constexpr glm::vec3 world_up { 0.f, 1.f, 0.f };

        float pitch = 0.f; // Camera pitch angle in radians, must be in the range of (-π/2, π/2).
        float yaw = 0.f; // Camera yaw angle in radians.

        mutable std::optional<glm::vec3> front, right, up;

        void invalidate_cache() noexcept;

    public:
        // View matrix parameters.
        glm::vec3 target = glm::zero<glm::vec3>(); // Target position, rendered in the center of the screen.
        float distance = 1.f; // Distance from target to camera.

        /**
         * @brief Get camera's normalized front direction vector.
         * @return Normalized front direction vector.
         */
        [[nodiscard]] glm::vec3 getFront() const noexcept;

        /**
         * @brief Get camera's normalized right direction vector.
         * @return Normalized right direction vector.
         */
        [[nodiscard]] glm::vec3 getRight() const noexcept;

        /**
         * @brief Get camera's normalized up direction vector.
         * @return Normalized up direction vector.
         */
        [[nodiscard]] glm::vec3 getUp() const noexcept;

        /**
         * @brief Get camera's position.
         * @return Camera's position.
         */
        [[nodiscard]] glm::vec3 getPosition() const noexcept;

        /**
         * @brief Get view matrix of the camera.
         * @return 4x4 view matrix.
         */
        [[nodiscard]] glm::mat4 getMatrix() const noexcept;

        /**
         * @brief Get camera pitch angle.
         * @return Camera pitch angle in radian, bounded in (-π/2, π/2).
         */
        float getPitch() const noexcept;

        /**
         * @brief Add camera pitch angle with bound constraint.
         * @param amount Addition amount in radians.
         * @note This function ensures that the updated pitch angle is in the range of (-π/2, π/2).
         */
        void addPitch(float amount) noexcept;

        /**
         * @brief Get camera yaw angle.
         * @return Camera yaw angle in radian, bounded in [0, 2π).
         */
        float getYaw() const noexcept;

        /**
         * @brief Add camera yaw angle with bound constraint.
         * @param amount Addition amount in radians.
         * @note This function ensures that the updated yaw angle is in the range of [0, 2π).
         */
        void addYaw(float amount) noexcept;
    };

    struct CameraProjection{};

    struct PerspectiveProjection : CameraProjection {
        float fov = glm::pi<float>() / 4; // Field of view in radians, must be in the range of (0, π).
        float near_distance = 1e-2f; // Near distance from camera to target. If the target is closer than near_distance, it is clipped.
        float far_distance = 1e4f; // Far distance from camera to target. If the target is farther than far_distance, it is clipped.

        /**
         * @brief Get perspective projection matrix of the camera.
         * @param region_aspect_ratio Aspect ratio of the region size.
         * @return 4x4 perspective projection matrix.
         */
        [[nodiscard]] glm::mat4 getMatrix(float region_aspect_ratio) const noexcept;
    };

    struct OrthographicProjection : CameraProjection {
        float near_distance = 1e-2f; // Near distance from camera to target. If the target is closer than near_distance, it is clipped.
        float far_distance = 1e4f; // Far distance from camera to target. If the target is farther than far_distance, it is clipped.

        /**
         * @brief Get orthographic projection matrix of the camera.
         * @param region_position Position of the region rectangle.
         * @param region_size Size of the region rectangle, must be positive.
         * @return 4x4 orthographic projection matrix.
         */
        [[nodiscard]] glm::mat4 getMatrix(glm::vec2 region_position, glm::vec2 region_size) const noexcept;
    };

    struct PerspectiveCamera{
        CameraView view;
        PerspectiveProjection projection;
    };

    struct OrthographicCamera{
        CameraView view;
        OrthographicProjection projection;
    };
}