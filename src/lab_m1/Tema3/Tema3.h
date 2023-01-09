#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace p3
{
    class GameObject;
    class Camera;
    class Player;

    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = nullptr);
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
        std::unordered_map<std::string, Texture2D*> textures;
        const std::string sourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "Tema3");
        const std::string sourcePrimitiveDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives");
        glm::mat4 projectionMatrix;
        Player* player;
    protected:
        std::vector<GameObject*> objectsToAdd;
        Camera* camera;
    };
}