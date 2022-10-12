#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    playerPos = new glm::vec3(0, 0.5f, 0);
    moveDir = new glm::vec3(0);
    cubePos = new glm::vec3(0);
    cubeMoveDir = new glm::vec3(0);
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.
    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    currentMesh = meshes["box"];
    glClearColor(0, 0, 0, 1);
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], *cubePos);

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
    RenderMesh(currentMesh, *playerPos, glm::vec3(0.1f));

    *playerPos += *moveDir * speed * deltaTimeSeconds;
    *cubePos += *cubeMoveDir * speed * deltaTimeSeconds;
    *cubeMoveDir = glm::vec3(0);
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    *moveDir = glm::vec3(0);
    if (window->KeyHold(GLFW_KEY_W))
    {
        moveDir->z = 1;
    }
    else if (window->KeyHold(GLFW_KEY_S))
    {
        moveDir->z = -1;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        moveDir->x = 1;
    }
    else if (window->KeyHold(GLFW_KEY_D))
    {
        moveDir->x = -1;
    }
    if (window->KeyHold(GLFW_KEY_Q))
    {
        moveDir->y = 1;
    }
    else if (window->KeyHold(GLFW_KEY_E))
    {
        moveDir->y = -1;
    }
    if (*moveDir != glm::vec3(0))
    {
        *moveDir = glm::normalize(*moveDir);
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        glClearColor(red.r, red.g, red.b, red.a);
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.
    if (key == GLFW_KEY_U)
    {
        if (currentMesh == meshes["box"])
        {
            currentMesh = meshes["bunny"];
        }
        else if (currentMesh == meshes["bunny"])
        {
            currentMesh = meshes["sphere"];
        }
        else if (currentMesh == meshes["sphere"])
        {
            currentMesh = meshes["box"];
        }
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    cubeMoveDir->x = deltaX;
    cubeMoveDir->z = deltaY;
    if (*cubeMoveDir != glm::vec3(0))
    {
        *cubeMoveDir = glm::normalize(*cubeMoveDir);
    }
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
