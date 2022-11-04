#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translate matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        glm::mat4 trans = glm::mat4(
            1, 0, 0, translateX,
            0, 1, 0, translateY,
            0, 0, 1, translateZ,
            0, 0, 0, 1
        );
        return glm::transpose(trans);

    }

    // Scale matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        glm::mat4 scale = glm::mat4(
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, scaleZ, 0,
            0, 0, 0, 1
        );
        return glm::transpose(scale);
    }

    // Rotate matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        glm::mat4 rotZ = glm::mat4(
            glm::cos(radians), -glm::sin(radians), 0, 0,
            glm::sin(radians), glm::cos(radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
        return glm::transpose(rotZ);
    }

    // Rotate matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        glm::mat4 rotY = glm::mat4(
            glm::cos(radians), 0, glm::sin(radians), 0,
            0, 1, 0, 0,
            -glm::sin(radians), 0, glm::cos(radians), 0,
            0, 0, 0, 1
        );
        return glm::transpose(rotY);

    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        glm::mat4 rotX = glm::mat4(
            1, 0, 0, 0,
            0, glm::cos(radians), -glm::sin(radians), 0,
            0, glm::sin(radians), glm::cos(radians), 0,
            0, 0, 0, 1
        );
        return glm::transpose(rotX);
    }
}   // namespace transform3D
