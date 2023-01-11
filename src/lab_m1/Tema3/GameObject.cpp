#include "GameObject.h"
#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>

using namespace p3;

GameObject::GameObject()
{
    Position = glm::vec3(0);
    Rotation = glm::vec3(0);
    Scale = glm::vec3(1);
}

GameObject::~GameObject() = default;

GameObject::GameObject(Tema3* scene) : GameObject()
{
    Scene = scene;
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

void GameObject::OnCollisionEnter(GameObject* gameObject)
{
}

Mesh* GameObject::CreateMesh(const char* name, const std::vector<glm::vec3>& positions,
                             const std::vector<glm::vec3>& normals,
                             const std::vector<glm::vec2>& texCoords,
                             const std::vector<unsigned int>& indices)
{
    // Mesh information is saved into a Mesh object
    Meshes[name] = new Mesh(name);
    Meshes[name]->InitFromData(positions, normals, texCoords, indices);
    return Meshes[name];
}

glm::vec2 GameObject::GetScreenPosition() const
{
    return Scene->GetCamera()->projectionMatrix * Scene->GetCamera()->GetViewMatrix() * ModelMatrix * glm::vec4(Position, 1.0);
}

void GameObject::SetPosition(const glm::vec3 pos)
{
	SetModelMatrix(pos, Rotation, Scale);
}

void GameObject::SetRotation(const glm::vec3 rot)
{
	Forward = glm::rotateX(glm::vec3(0, 0, 1), rot.x);
	Forward = glm::rotateY(Forward, rot.y);
	Forward = glm::rotateZ(Forward, rot.z);
	SetModelMatrix(Position, rot, Scale);
}

void GameObject::SetScale(const glm::vec3 sc)
{
	SetModelMatrix(Position, Rotation, sc);
}

glm::mat4 GameObject::TranslateMatrix(const glm::mat4 modelMatrix, const glm::vec3 translation)
{
    const glm::mat4 trans = glm::mat4(
        1, 0, 0, translation.x,
        0, 1, 0, translation.y,
        0, 0, 1, translation.z,
        0, 0, 0, 1
    );
    return modelMatrix * glm::transpose(trans);
}

glm::mat4 GameObject::ScaleMatrix(const glm::mat4 modelMatrix, const glm::vec3 scale)
{
    const glm::mat4 scaleMat = glm::mat4(
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1
    );
	return modelMatrix * glm::transpose(scaleMat);
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
    Position = pos;
    Rotation = rot;
    Scale = sc;
    ModelMatrix = glm::mat4(1) * glm::transpose(trans) * glm::transpose(rotZ * rotY * rotX) * glm::transpose(scaleMat);
}
