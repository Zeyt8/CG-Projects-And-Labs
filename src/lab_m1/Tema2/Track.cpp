#include "Track.h"

using namespace p2;

Track::Track(Tema2* scene) : GameObject(scene)
{
	points.push_back(glm::vec3(2.26f, 0, -1.66f));
	points.push_back(glm::vec3(0.72f, 0, -2.34f));
	points.push_back(glm::vec3(-0.86f, 0, -1.82f));
	points.push_back(glm::vec3(-2.22f, 0, -1.8f));
	points.push_back(glm::vec3(-3.32f, 0, -2.12f));
	points.push_back(glm::vec3(-4.38f, 0, -1.02f));
	points.push_back(glm::vec3(-4.66f, 0, -0.1f));
	points.push_back(glm::vec3(-4.26f, 0, 0.56f));
	points.push_back(glm::vec3(-3.08f, 0, 0.5f));
	points.push_back(glm::vec3(-1.96f, 0, -0.5f));
	points.push_back(glm::vec3(-0.54f, 0, -0.66f));
	points.push_back(glm::vec3(0.56f, 0, 0.22f));
	points.push_back(glm::vec3(1.0f, 0, 1.3f));
	points.push_back(glm::vec3(0.88f, 0, 2.56f));
	points.push_back(glm::vec3(1.98f, 0, 3.46f));
	points.push_back(glm::vec3(3.2f, 0, 2.84f));
	points.push_back(glm::vec3(3.16f, 0, 1.48f));
	points.push_back(glm::vec3(2.98f, 0, -0.24f));

	for (int i = 0; i < points.size(); i++)
	{
		points[i] *= 10;
	}

	std::vector<VertexFormat> vertices;

	std::vector<unsigned int> indices;
	for (int i = 0; i < points.size() - 1; i++)
	{
		glm::vec3 d = points[i + 1] - points[i];
		glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));
		p = glm::normalize(p);
		vertices.push_back(VertexFormat(points[i] - p));
		vertices.push_back(VertexFormat(points[i] + p));
	}
	glm::vec3 d = points[0] - points[points.size() - 1];
	glm::vec3 p = glm::cross(d, glm::vec3(0, 1, 0));
	p = glm::normalize(p);
	vertices.push_back(VertexFormat(points[points.size() - 1] - p));
	vertices.push_back(VertexFormat(points[points.size() - 1] + p));

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
}

void Track::Start()
{
}

void Track::Update(float deltaTime)
{
	scene->RenderMesh(meshes["track"], scene->GetShader("VertexColor"), modelMatrix);
}
