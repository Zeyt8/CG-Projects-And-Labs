#pragma once
#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class GameObject;
    class Duck;
    class Player;

    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;
        virtual void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        Shader* GetShader(std::string shader)
        {
            return shaders[shader];
        }
        gfxc::Camera* GetSceneCamera()
        {
            return SimpleScene::GetSceneCamera();
        }
        WindowObject* GetWindow()
        {
            return window;
        }

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

    private:
        std::vector<GameObject*> gameObjects;
        Duck* duck;
        Player* player;

        bool isDuckInScene = false;
        float timeSinceDuck = 0;

        void Shoot(int mouseX, int mouseY);
    };
}