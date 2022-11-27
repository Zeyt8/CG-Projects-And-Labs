#pragma once

#include "GameObject.h"

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
            distanceToTarget = 4;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        virtual ~Camera()
        { }

        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void TranslateForward(const float distance)
        {
            position += forward * distance;
        }

        void RotateThirdPerson_OX(const float angle)
        {
            TranslateForward(distanceToTarget);
            forward = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
            forward = glm::normalize(forward);
            up = glm::cross(right, forward);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(const float angle)
        {
            TranslateForward(distanceToTarget);
            forward = glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1));
            forward = glm::normalize(forward);
            right = glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1));
            right = glm::normalize(right);
            up = glm::cross(right, forward);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(const float angle)
        {
            TranslateForward(distanceToTarget);
            right = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
            right = glm::normalize(forward);
			up = glm::cross(forward, right);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix() const
        {
            return glm::lookAt(position, position + forward, up);
        }

        void Awake() override
        {
        }

		void Start() override
		{
		}

		void Update(float deltaTime) override
		{
			if (target != nullptr)
			{
				position = target->position - forward * distanceToTarget;
			}
		}

    public:
    	GameObject* target;
        float distanceToTarget;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
