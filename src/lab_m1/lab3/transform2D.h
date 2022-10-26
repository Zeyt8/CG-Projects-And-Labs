#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        glm::mat3 translate = glm::mat3(1, 0, translateX, 0, 1, translateY, 0, 0, 1);
        return glm::transpose(translate);

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        glm::mat3 scale = glm::mat3(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1);
        return glm::transpose(scale);
    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        glm::mat3 rotate = glm::mat3(glm::cos(radians), -glm::sin(radians), 0, glm::sin(radians), glm::cos(radians), 0, 0, 0, 1);
        return glm::transpose(rotate);
    }
}   // namespace transform2D
