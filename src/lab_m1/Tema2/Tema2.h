#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace p2
{
    class GameObject;
    class Camera;
    class Car;

    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 pos);
        Shader* GetShader(const std::string& shader)
        {
            return shaders[shader];
        }
		void SetCamera(Camera* camera)
		{
			this->camera = camera;
		}
        Camera* GetCamera() const
        {
            return camera;
        }
        WindowObject* GetWindow() const
        {
            return window;
        }
        void AddObject(GameObject* object);

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    public:
        std::vector<GameObject*> gameObjects;
        glm::mat4 projectionMatrix;
        std::vector<Car*> enemies;
    protected:
        std::vector<GameObject*> objectsToAdd;
        Camera* camera;
        Car* player;
    };
}