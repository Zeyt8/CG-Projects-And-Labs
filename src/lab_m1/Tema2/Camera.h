#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "lab_m1/Tema2/GameObject.h"

namespace p2
{
    class Camera : public GameObject
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void TranslateForward(float distance)
        {
            position += forward * distance;
        }

        void RotateThirdPerson_OX(float angle)
        {
            TranslateForward(distanceToTarget);
            forward = glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(forward, 1));
            forward = glm::normalize(forward);
            up = glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(up, 1));
            up = glm::normalize(up);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(distanceToTarget);
            forward = glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1));
            forward = glm::normalize(forward);
            up = glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 1));
            up = glm::normalize(up);
            right = glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1));
            right = glm::normalize(right);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(distanceToTarget);
            up = glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(up, 1));
            up = glm::normalize(up);
            right = glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1));
            right = glm::normalize(right);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
