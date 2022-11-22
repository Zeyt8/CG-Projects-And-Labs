#include "lab_m1/Tema2/GameObject.h"

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

void GameObject::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

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

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->SetDrawMode(GL_TRIANGLES);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void GameObject::SetPosition(glm::vec3 pos)
{
    glm::mat4 trans = glm::mat4(
        1, 0, 0, -position.x + pos.x,
        0, 1, 0, -position.y + pos.y,
        0, 0, 1, -position.z + pos.z,
        0, 0, 0, 1
    );
    position = pos;
    modelMatrix = trans * modelMatrix;
}

void GameObject::SetRotation(glm::vec3 rot)
{
    glm::mat4 rotX = glm::mat4(
        1, 0, 0, 0,
        0, glm::cos(-rotation.x + rot.x), -glm::sin(-rotation.x + rot.x), 0,
        0, glm::sin(-rotation.x + rot.x), glm::cos(-rotation.x + rot.x), 0,
        0, 0, 0, 1
    );
    glm::mat4 rotY = glm::mat4(
        glm::cos(-rotation.y + rot.y), 0, glm::sin(-rotation.y + rot.y), 0,
        0, 1, 0, 0,
        -glm::sin(-rotation.y + rot.y), 0, glm::cos(-rotation.y + rot.y), 0,
        0, 0, 0, 1
    );
    glm::mat4 rotZ = glm::mat4(
        glm::cos(-rotation.z + rot.z), -glm::sin(-rotation.z + rot.z), 0, 0,
        glm::sin(-rotation.z + rot.z), glm::cos(-rotation.z + rot.z), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    rotation = rot;
    modelMatrix = rot.x * rot.y * rotZ * modelMatrix;
}

void GameObject::SetScale(glm::vec3 sc)
{
    glm::mat4 scaleMat = glm::mat4(
        -scale.x + sc.x, 0, 0, 0,
        0, -scale.y + sc.y, 0, 0,
        0, 0, -scale.z + sc.z, 0,
        0, 0, 0, 1
    );
    scale = sc;
    modelMatrix = scaleMat * modelMatrix;
}
