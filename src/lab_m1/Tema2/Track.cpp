#include "Track.h"
#include "Tree.h"
#include <iostream>

using namespace p2;

std::vector<glm::vec3> Track::paths[] = { std::vector<glm::vec3>(), std::vector<glm::vec3>() };
Track* Track::Instance = nullptr;

Track::Track(Tema2* scene) : GameObject(scene)
{
	Instance = this;

	points.emplace_back(2.26f, 0, -1.66f);
	points.emplace_back(0.82f, 0, -2.04f);
	points.emplace_back(-1.07f, 0, -1.76f);
	points.emplace_back(-2.51f, 0, -1.63f);
	points.emplace_back(-3.22f, 0, -1.05f);
	points.emplace_back(-3.51f, 0, -0.21f);
	points.emplace_back(-3.25f, 0, 0.61f);
	points.emplace_back(-2.0f, 0, 1.0f);
	points.emplace_back(-0.76f, 0, 0.72f);
	points.emplace_back(0.0f, 0, 0.23f);
	points.emplace_back(0.67f, 0, 0.0f);
	points.emplace_back(1.38f, 0, 0.15f);
	points.emplace_back(1.67f, 0, 0.9f);
	points.emplace_back(2.0f, 0, 2.0f);
	points.emplace_back(2.64f, 0, 2.21f);
	points.emplace_back(3.35f, 0, 2.04f);
	points.emplace_back(3.49f, 0, 1.39f);
	points.emplace_back(3.18f, 0, 0.68f);
	points.emplace_back(2.75f, 0, -0.48f);

	for (glm::vec3& point : points)
	{
		point *= 15;
	}
	
	// Generate inner and outer points
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < points.size(); i++)
	{
		glm::vec3 d = points[(i + 1) % points.size()] - points[i];
		glm::vec3 p = glm::normalize(glm::cross(d, glm::vec3(0, 1, 0)));
		glm::vec3 d2 = points[(i + 2) % points.size()] - points[(i + 1) % points.size()];
		glm::vec3 p3 = glm::normalize(glm::cross(d2, glm::vec3(0, 1, 0)));

		// Subdivide
		for (int j = 0; j < 10; j++)
		{
			glm::vec3 p1 = points[i] - p * 4.0f;
			glm::vec3 p2 = points[(i + 1) % points.size()] - p3 * 4.0f;
			vertices.emplace_back(p1 + (p2 - p1) / 10.0f * (float)j, glm::vec3(0.2f, 0.2f, 0.19f));

			p1 = points[i] + p * 4.0f;
			p2 = points[(i + 1) % points.size()] + p3 * 4.0f;
			vertices.emplace_back(p1 + (p2 - p1) / 10.0f * (float)j, glm::vec3(0.2f, 0.2f, 0.19f));

			p1 = points[i] - p * 2.0f;
			p2 = points[(i + 1) % points.size()] - p3 * 2.0f;
			paths[0].push_back(p1 + (p2 - p1) / 10.0f * (float)j);

			p1 = points[i] + p * 2.0f;
			p2 = points[(i + 1) % points.size()] + p3 * 2.0f;
			paths[1].push_back(p1 + (p2 - p1) / 10.0f * (float)j);
		}
	}
	
	// Generate faces
	std::vector<unsigned int> indices;
	for (int i = 0; i < vertices.size() - 3; i += 2)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(i + 2);
	}
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);
	indices.push_back(0);
	indices.push_back(vertices.size() - 1);
	indices.push_back(1);
	indices.push_back(0);

	meshes["track"] = new Mesh("track");
	meshes["track"]->InitFromData(vertices, indices);

	CreateMesh("track", vertices, indices);
}

void Track::Awake()
{
	for (int i = 0; i < points.size() - 1; i++)
	{
		glm::vec3 d = points[i + 1] - points[i];
		glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));
		p = glm::normalize(p);
		{
			Tree* tree = new Tree(scene);
			tree->SetPosition(points[i] - p * 7.0f);
			scene->AddObject(tree);
		}
		{
			Tree* tree = new Tree(scene);
			tree->SetPosition(points[i] + p * 7.0f);
			scene->AddObject(tree);
		}
	}
}

void Track::Start()
{
}

void Track::Update(float deltaTime)
{
}

void Track::Render()
{
	scene->RenderMesh(meshes["track"], scene->GetShader("Curve"), modelMatrix, position);
}
