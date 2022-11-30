#include "GameObject.h"

#include <iostream>

using namespace p2;

p2::GameObject::GameObject()
{
}

GameObject::GameObject(Tema2* scene)
{
    position = glm::vec3(0);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);
    GameObject::scene = scene;
}

GameObject::~GameObject()
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

Mesh* GameObject::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
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
