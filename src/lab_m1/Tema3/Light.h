#pragma once

#include "GameObject.h"

namespace p3
{
	enum LightTypes
	{
		Directional,
		Spot,
		Point
	};

	struct Light : GameObject
	{
		Light(const LightTypes lightType, const glm::vec3 position = glm::vec3(0),
			const glm::vec3 color = glm::vec3(1), const glm::vec3 rotation = glm::vec3(0), const float angle = -1)
		{
			LightType = lightType;
			SetPosition(position);
			Color = color;
			SetRotation(rotation);
			Angle = angle;
		}

		LightTypes LightType;

		glm::vec3 Color;
		float Intensity = 1;
		float Angle = -1;
	};
}