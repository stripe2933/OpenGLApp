//
// Created by gomkyung2 on 2023/08/29.
//

#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/constants.hpp>

namespace OpenGL{
    struct Camera{
        static constexpr glm::vec3 up { 0.f, 1.f, 0.f };

        // View matrix parameters.
        float pitch = 0.f; // Camera pitch angle in radians, must be in the range of (-π/2, π/2).
        float yaw = 0.f; // Camera yaw angle in radians.
        glm::vec3 target = glm::zero<glm::vec3>(); // Target position, rendered in the center of the screen.
        float distance = 1.f; // Distance from target to camera.

        // Projection matrix parameters.
        float fov = glm::pi<float>() / 4; // Field of view in radians, must be in the range of (0, π).
        float min_distance = 1e-2f; // Min distance from camera to target. If the target is closer than min_distance, it is clipped.
        float max_distance = 1e4f; // Max distance from camera to target. If the target is farther than min_distance, it is clipped.

        /**
         * @brief Get camera's normalized front direction vector.
         * @return Normalized front direction vector.
         */
        [[nodiscard]] glm::vec3 getFront() const noexcept;

        /**
         * @brief Get camera's position.
         * @return Camera's position.
         */
        [[nodiscard]] glm::vec3 getPosition() const noexcept;

        /**
         * @brief Get view matrix of the camera.
         * @return 4x4 view matrix.
         */
        [[nodiscard]] glm::mat4 getView() const noexcept;

        /**
         * @brief Get perspective projection matrix of the camera.
         * @param region_size Size of the render region, must be positive.
         * @return 4x4 perspective projection matrix.
         */
        [[nodiscard]] glm::mat4 getProjection(float aspect_ratio) const noexcept;

        /**
         * @brief Add camera pitch angle with bound constraint.
         * @param amount Addition amount in radians.
         * @note You can directly set \p pitch member field, but this function ensures that the updated pitch angle is in the range of (-π/2, π/2).
         */
        void addPitch(float amount) noexcept;
    };
};