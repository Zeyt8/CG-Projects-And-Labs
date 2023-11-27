#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "stb/stb_image.h"

using namespace std;
using namespace m2;

struct Particle
{
    int id;
    float speed;
    float delay;
    float initialDelay;
    float lifetime;
    float initialLifetime;

    Particle() {}

    Particle(int id, float speed)
    {
        SetInitial(id, speed);
    }

    void SetInitial(int id, float speed, float delay = 0, float lifetime = 0)
    {
        this->id = id;
        this->speed = speed;

        this->delay = delay;
        initialDelay = delay;

        this->lifetime = lifetime;
        initialLifetime = lifetime;
    }
};

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    framebuffer_object = 0;
    color_texture = 0;
    depth_texture = 0;

    angle = 0;

    type = 0;

    mirrorPosition = glm::vec3(0);
    mirrorRotation = glm::vec3(0);
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, -1, 4), glm::quat(glm::vec3(RADIANS(10), 0, 0)));
    camera->Update();

    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube");
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("archer");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "characters", "archer"), "Archer.fbx");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering cubemap texture
    {
        Shader* shader = new Shader("CubeMap");
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "CubeMap.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for standard rendering
    {
        Shader* shader = new Shader("ShaderNormal");
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for creating a CUBEMAP
    {
        Shader* shader = new Shader("Framebuffer");
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Framebuffer.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "pos_x.png"),
        PATH_JOIN(texturePath, "pos_y.png"),
        PATH_JOIN(texturePath, "pos_z.png"),
        PATH_JOIN(texturePath, "neg_x.png"),
        PATH_JOIN(texturePath, "neg_y.png"),
        PATH_JOIN(texturePath, "neg_z.png"));

    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS), "characters", "archer", "Akai_E_Espiritu.fbm", "akai_diffuse.png");

    // Create the framebuffer on which the scene is rendered from the perspective of the mesh
    // Texture size must be cubic
    CreateFramebuffer(1024, 1024);

    TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "particle2.png");
    LoadShader("Fireflies", "Particle_fireworks", "Particle_simple", "Particle", true);

    // generate particle effect
    int nrParticles = 100;
    particleEffect = new ParticleEffect<Particle>();
    particleEffect->Generate(nrParticles, true);

    auto particleSSBO = particleEffect->GetParticleBuffer();
    Particle* data = const_cast<Particle*>(particleSSBO->GetBuffer());

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        float speed = RandomFloat(0.2f, 0.4f);

        data[i].SetInitial(i / (nrParticles / 5), speed, RandomFloat(10, 20), RandomFloat(2, 5));
    }

    particleSSBO->SetBufferData(data);
}


void Tema1::FrameStart()
{
}


void Tema1::Update(float deltaTimeSeconds)
{
    angle += 0.5f * deltaTimeSeconds;

    auto camera = GetSceneCamera();

    // Draw the scene in Framebuffer
    if (framebuffer_object)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
        // Set the clear color for the color buffer
        glClearColor(0, 0, 0, 1);
        // Clears the color buffer (using the previously set color) and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, 1024, 1024);

        Shader* shader = shaders["Framebuffer"];
        shader->Use();

        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);

        {
            glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
            glUniform1i(glGetUniformLocation(shader->program, "texture_cubemap"), 1);

            glUniform1i(glGetUniformLocation(shader->program, "cube_draw"), 1);

            meshes["cube"]->Render();
        }

        glm::mat4 cubeView[6] =
        {
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +X
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -X
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(0.0f,-1.0f, 0.0f), glm::vec3(0.0f, 0.0f,-1.0f)), // -Y
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // +Z
            glm::lookAt(mirrorPosition, mirrorPosition + glm::vec3(0.0f, 0.0f,-1.0f), glm::vec3(0.0f,-1.0f, 0.0f)), // -Z
        };

        for (int i = 0; i < 5; i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= glm::rotate(glm::mat4(1), angle + i * glm::radians(360.0f) / 5, glm::vec3(0, 1, 0));
            modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(3, -1, 0));
            modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
            modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(0.01f));

            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

            glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureManager::GetTexture("Akai_E_Espiritu.fbm\\akai_diffuse.png")->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

            glUniform1i(glGetUniformLocation(shader->program, "cube_draw"), 0);
            glUniform1i(glGetUniformLocation(shader->program, "type"), type);

            meshes["archer"]->Render();
        }

        shader = shaders["Fireflies"];
        shader->Use();
        {
            glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform1i(glGetUniformLocation(shader->program, "type"), type);
            glUniform1f(glGetUniformLocation(shader->program, "deltaTime"), deltaTimeSeconds);

            TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
            particleEffect->Render(GetSceneCamera(), shader);
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, color_texture);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        //reset drawing to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);

    // Draw the cubemap
    {
        Shader* shader = shaders["ShaderNormal"];
        shader->Use();

        glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        int loc_texture = shader->GetUniformLocation("texture_cubemap");
        glUniform1i(loc_texture, 0);

        meshes["cube"]->Render();
    }

    // Draw five archers around the mesh
    for (int i = 0; i < 5; i++)
    {
        Shader* shader = shaders["Simple"];
        shader->Use();

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= glm::rotate(glm::mat4(1), angle + i * glm::radians(360.0f) / 5, glm::vec3(0, 1, 0));
        modelMatrix *= glm::translate(glm::mat4(1), glm::vec3(3, -1, 0));
        modelMatrix *= glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(0, 1, 0));
        modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(0.01f));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureManager::GetTexture("Akai_E_Espiritu.fbm\\akai_diffuse.png")->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

        meshes["archer"]->Render();
    }

    // Draw the reflection on the mesh
    {
        Shader* shader = shaders["CubeMap"];
        shader->Use();

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= glm::translate(glm::mat4(1), mirrorPosition);
        modelMatrix *= glm::rotate(glm::mat4(1), mirrorRotation.x, glm::vec3(1, 0, 0));
        modelMatrix *= glm::rotate(glm::mat4(1), mirrorRotation.y, glm::vec3(0, 1, 0));
        modelMatrix *= glm::rotate(glm::mat4(1), mirrorRotation.z, glm::vec3(0, 0, 1));
        modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(2.5f));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        auto cameraPosition = camera->m_transform->GetWorldPosition();

        if (!color_texture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
            int loc_texture = shader->GetUniformLocation("texture_cubemap");
            glUniform1i(loc_texture, 0);
        }

        if (color_texture) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, color_texture);
            int loc_texture2 = shader->GetUniformLocation("texture_cubemap");
            glUniform1i(loc_texture2, 1);
        }


        int loc_camera = shader->GetUniformLocation("camera_position");
        glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        meshes["quad"]->Render();
    }
}


void Tema1::FrameEnd()
{
    // DrawCoordinateSystem();
}


unsigned int Tema1::UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z)
{
    int width, height, chn;

    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID = 0;
    // Create the texture
    glGenTextures(1, &textureID);
    // Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic) {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load texture information for each face
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
    }

    // Free memory
    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}

void Tema1::CreateFramebuffer(int width, int height)
{
    // In this method, use the attributes
    // 'framebuffer_object', 'color_texture'
    // declared in lab6.h

    // Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    // Generate and bind the color texture
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, color_texture);
    // Initialize the color textures
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    if (color_texture) {
        //cubemap params
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (GLEW_EXT_texture_filter_anisotropic) {
            float maxAnisotropy;

            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Bind the color textures to the framebuffer as a color attachments
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        std::vector<GLenum> draw_textures;
        draw_textures.push_back(GL_COLOR_ATTACHMENT0);
        glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    }

    // Generate and bind the depth texture
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depth_texture);
    // Initialize the depth textures
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    if (depth_texture) {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);
    }

    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
    if (window->KeyHold(GLFW_KEY_UP)) {
        mirrorPosition += glm::vec3(0, 0, 1) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        mirrorPosition += glm::vec3(0, 0, -1) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        mirrorPosition += glm::vec3(1, 0, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        mirrorPosition += glm::vec3(-1, 0, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_N)) {
        mirrorPosition += glm::vec3(0, 1, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_M)) {
        mirrorPosition += glm::vec3(0, -1, 0) * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        mirrorRotation += glm::vec3(0, 0, 1) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_J)) {
        mirrorRotation += glm::vec3(0, 0, -1) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_H)) {
        mirrorRotation += glm::vec3(1, 0, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_K)) {
        mirrorRotation += glm::vec3(-1, 0, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Y)) {
        mirrorRotation += glm::vec3(0, 1, 0) * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_I)) {
        mirrorRotation += glm::vec3(0, -1, 0) * deltaTime;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_1)
    {
        type = 0;
    }

    if (key == GLFW_KEY_2)
    {
        type = 1;
    }

    if (key == GLFW_KEY_3)
    {
        type = 2;
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}

void Tema1::LoadShader(const std::string& name, const std::string& VS, const std::string& FS, const std::string& GS, bool hasGeomtery)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    // Create a shader program for particle system
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, VS + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, FS + ".FS.glsl"), GL_FRAGMENT_SHADER);
        if (hasGeomtery)
        {
            shader->AddShader(PATH_JOIN(shaderPath, GS + ".GS.glsl"), GL_GEOMETRY_SHADER);
        }

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

float Tema1::RandomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}