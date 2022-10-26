#include "lab_m1/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab3::Lab3()
{
}


Lab3::~Lab3()
{
}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(0, 1, 0));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0, 1));
    AddMeshToList(square3);

    Mesh* roata1 = object2D::CreateSquare("roata1", corner, 50, glm::vec3(0, 0, 1), true);
    AddMeshToList(roata1);

    Mesh* roata2 = object2D::CreateSquare("roata2", corner, 50, glm::vec3(0, 0, 1), true);
    AddMeshToList(roata2);

    Mesh* corp1 = object2D::CreateSquare("corp1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(corp1);

    Mesh* corp2 = object2D::CreateSquare("corp2", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(corp2);

    Mesh* corp3 = object2D::CreateSquare("corp3", corner, 60, glm::vec3(1, 0, 0), true);
    AddMeshToList(corp3);
}


void Lab3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    // Update steps for translation, rotation and scale,
    // in order to create animations. Use the class variables in the
    // class header, and if you need more of them to complete the task,
    // add them over there!
    translateX += 10 * deltaTimeSeconds;
    translateY += 10 * deltaTimeSeconds;
    scaleX += 0.1f * deltaTimeSeconds;
    scaleY += 0.1f * deltaTimeSeconds;
    angularStep += 0.1f * deltaTimeSeconds;

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(translateX, translateY);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2D::Translate(cx, cy) * transform2D::Scale(scaleX, scaleY) * transform2D::Translate(-cx, -cy);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2D::Translate(cx, cy) * transform2D::Rotate(angularStep) * transform2D::Translate(-cx, -cy);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    //car
    glm::mat3 roataPos = glm::mat3(1);
    glm::mat3 roataPos2 = glm::mat3(1);
    glm::mat3 corpPos = glm::mat3(1);
    roataPos *= transform2D::Translate(translateX * 2 + 30, 10) * transform2D::Translate(25, 25) * transform2D::Rotate(-angularStep * 2) * transform2D::Translate(-25, -25);
    RenderMesh2D(meshes["roata1"], shaders["VertexColor"], roataPos);
    roataPos2 *= transform2D::Translate(translateX * 2 + 130, 10) * transform2D::Translate(25, 25) * transform2D::Rotate(-angularStep * 2) * transform2D::Translate(-25, -25);
    RenderMesh2D(meshes["roata2"], shaders["VertexColor"], roataPos2);

    corpPos *= transform2D::Translate(translateX * 2, 50) * transform2D::Scale(2, 1);
    RenderMesh2D(meshes["corp1"], shaders["VertexColor"], corpPos);
    corpPos *= transform2D::Translate(20, 70);
    RenderMesh2D(meshes["corp3"], shaders["VertexColor"], corpPos);
}


void Lab3::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab3::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab3::OnWindowResize(int width, int height)
{
}
