#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace p3
{
    class GameObject;
    class Camera;
    class Player;
    struct Light;

    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = nullptr, bool time = false);
        Shader* GetShader(const std::string& shader)
        {
            return shaders[shader];
        }
		void SetCamera(Camera* camera)
		{
			_camera = camera;
		}
        Camera* GetCamera() const
        {
            return _camera;
        }
        WindowObject* GetWindow() const
        {
            return window;
        }
        void AddObject(GameObject* object);
        static glm::vec3 ConvertToTiltedPlane(glm::vec3);

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
        std::vector<GameObject*> GameObjects;
        std::unordered_map<std::string, Texture2D*> Textures;
        const std::string SourceTextureDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "Tema3");
        const std::string SourcePrimitiveDir = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives");
        Player* PlayerObject;
        std::vector<Light*> Lights;
    protected:
        std::vector<GameObject*> _objectsToAdd;
        Camera* _camera;
    };
}