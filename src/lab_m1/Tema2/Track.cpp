#include "Track.h"
#include "Tree.h"

using namespace p2;

Track::Track(Tema2* scene) : GameObject(scene)
{
	points.emplace_back(2.26f, 0, -1.66f);
	points.emplace_back(0.72f, 0, -2.34f);
	points.emplace_back(-0.86f, 0, -1.82f);
	points.emplace_back(-2.22f, 0, -1.8f);
	points.emplace_back(-3.32f, 0, -2.12f);
	points.emplace_back(-4.38f, 0, -1.02f);
	points.emplace_back(-4.66f, 0, -0.1f);
	points.emplace_back(-4.26f, 0, 0.56f);
	points.emplace_back(-3.08f, 0, 0.5f);
	points.emplace_back(-1.96f, 0, -0.5f);
	points.emplace_back(-0.54f, 0, -0.66f);
	points.emplace_back(0.56f, 0, 0.22f);
	points.emplace_back(1.0f, 0, 1.3f);
	points.emplace_back(0.88f, 0, 2.56f);
	points.emplace_back(1.98f, 0, 3.46f);
	points.emplace_back(3.2f, 0, 2.84f);
	points.emplace_back(3.16f, 0, 1.48f);
	points.emplace_back(2.98f, 0, -0.24f);

	for (glm::vec3& point : points)
	{
		point *= 15;
	}
	
	// Subdivide track
	std::vector<glm::vec3> newPoints;
	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			newPoints.push_back(points[i] + (points[i + 1] - points[i]) / 5.0f * static_cast<float>(j));
		}
	}
	newPoints.push_back(points[points.size() - 1]);

	// Generate inner and outer points
	std::vector<VertexFormat> vertices;

	for (int i = 0; i < newPoints.size() - 1; i++)
	{
		glm::vec3 d = newPoints[i + 1] - newPoints[i];
		glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));
		p = glm::normalize(p);
		vertices.emplace_back(newPoints[i] - p * 4.0f, glm::vec3(0.2f, 0.2f, 0.19f));
		vertices.emplace_back(newPoints[i] + p * 4.0f, glm::vec3(0.2f, 0.2f, 0.19f));
	}
	const glm::vec3 d = newPoints[0] - newPoints[newPoints.size() - 1];
	glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));
	p = glm::normalize(p);
	vertices.emplace_back(newPoints[newPoints.size() - 1] - p * 5.0f, glm::vec3(0.2f, 0.2f, 0.19f));
	vertices.emplace_back(newPoints[newPoints.size() - 1] + p * 5.0f, glm::vec3(0.2f, 0.2f, 0.19f));
	
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
	scene->RenderMesh(meshes["track"], scene->GetShader("VertexColor"), modelMatrix);
}
