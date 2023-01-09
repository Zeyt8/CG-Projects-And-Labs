#include "GameObject.h"

#include <iostream>

using namespace p3;

GameObject::GameObject()
{
    position = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
}

GameObject::~GameObject()
{
}

GameObject::GameObject(Tema3* scene) : GameObject()
{
    GameObject::scene = scene;
}

void p3::GameObject::Awake()
{
}

void p3::GameObject::Start()
{
}

void p3::GameObject::Update(float deltaTime)
{
}

void GameObject::LateUpdate(float deltaTime)
{
}

void GameObject::OnInputUpdate(float deltaTime, int mods)
{
}

void GameObject::Render()
{
}

Mesh* GameObject::CreateMesh(const char* name, const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& normals,
    const std::vector<glm::vec2>& texCoords,
    const std::vector<unsigned int>& indices)
{
    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromData(positions, normals, texCoords, indices);
    return meshes[name];
}

void GameObject::SetPosition(glm::vec3 pos)
{
	SetModelMatrix(pos, rotation, scale);
}

void GameObject::SetRotation(glm::vec3 rot)
{
	SetModelMatrix(position, rot, scale);
}

void GameObject::SetScale(glm::vec3 sc)
{
	SetModelMatrix(position, rotation, sc);
}

void GameObject::SetModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 sc)
{
    const glm::mat4 trans = glm::mat4(
        1, 0, 0, pos.x,
        0, 1, 0, pos.y,
        0, 0, 1, pos.z,
        0, 0, 0, 1
    );

    const glm::mat4 rotX = glm::mat4(
        1, 0, 0, 0,
        0, glm::cos(rot.x), -glm::sin(rot.x), 0,
        0, glm::sin(rot.x), glm::cos(rot.x), 0,
        0, 0, 0, 1
    );
    const glm::mat4 rotY = glm::mat4(
        glm::cos(rot.y), 0, glm::sin(rot.y), 0,
        0, 1, 0, 0,
        -glm::sin(rot.y), 0, glm::cos(rot.y), 0,
        0, 0, 0, 1
    );
    const glm::mat4 rotZ = glm::mat4(
        glm::cos(rot.z), -glm::sin(rot.z), 0, 0,
        glm::sin(rot.z), glm::cos(rot.z), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    const glm::mat4 scaleMat = glm::mat4(
        sc.x, 0, 0, 0,
        0, sc.y, 0, 0,
        0, 0, sc.z, 0,
        0, 0, 0, 1
    );
    position = pos;
    rotation = rot;
    scale = sc;
    modelMatrix = glm::mat4(1) * glm::transpose(trans) * glm::transpose(rotX * rotY * rotZ) * glm::transpose(scaleMat);
}
