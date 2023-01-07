#pragma once

#include "GameObject.h"

namespace p3
{
    class Camera : public GameObject
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 0, 0);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 5;
        }

        virtual ~Camera()
        { }

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
			rotation.x += angle;
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
			rotation.y += angle;
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(const float angle)
        {
            TranslateForward(distanceToTarget);
            right = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
            right = glm::normalize(forward);
			up = glm::cross(forward, right);
			rotation.z += angle;
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix() const
        {
            return glm::lookAt(position, position + forward, up);
        }

        void Awake() override {}

		void Start() override {}

		void Update(float deltaTime) override
		{
			if (followTarget != nullptr)
			{
				position = followTarget->position - forward * distanceToTarget;
			}
            if (lookAtTarget != nullptr)
            {
                RotateThirdPerson_OY(lookAtTarget->rotation.y - rotation.y);
            }
		}

    public:
    	GameObject* followTarget = nullptr;
    	GameObject* lookAtTarget = nullptr;
        float distanceToTarget;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
