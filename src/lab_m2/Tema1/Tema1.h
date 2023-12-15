#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "core/gpu/particle_effect.h"

#include <string>

struct Particle;

namespace m2
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void CreateFramebuffer(int width, int height);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        unsigned int UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z);
        void LoadShader(const std::string& name,
            const std::string& VS, const std::string& FS, const std::string& GS = "",
            bool hasGeomtery = false);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        float RandomFloat(float a, float b);

    private:
        int cubeMapTextureID;
        float angle;
        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int depth_texture;
        unsigned int type;
        glm::vec3 mirrorPosition;
        glm::vec3 mirrorRotation;
        ParticleEffect<Particle>* particleEffect;
    };
}   // namespace m2
