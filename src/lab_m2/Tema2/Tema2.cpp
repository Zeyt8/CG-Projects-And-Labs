#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>
#include <windows.h>
#include <ppl.h>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    saveScreenToImage = false;
    window->SetSize(600, 600);
    processed = false;
    firstWhitePixel = glm::vec2(0, 0);
    lastWhitePixel = glm::vec2(0, 0);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "test_images", "star.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "test_images", "star.png"), nullptr, "newImage", true, true);
    watermark = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "test_images", "watermark.png"), nullptr, "watermark", true, true);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader("ImageProcessing");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DisplayImage");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "SimpleFragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    CreateFramebuffer(framebuffer_object, color_texture, originalImage->GetWidth(), originalImage->GetHeight());
    CreateFramebuffer(watermark_framebuffer, watermark_color_texture, watermark->GetWidth(), watermark->GetHeight());

    watermarkPixels.resize(4 * watermark->GetWidth() * watermark->GetHeight());
    bool isrgba = watermark->GetNrChannels() == 4;
    for (int i = 0, j = 0; i < watermark->GetWidth() * watermark->GetHeight(); i++)
    {
        watermarkPixels[4 * i] = watermark->GetImageData()[j];
        watermarkPixels[4 * i + 1] = watermark->GetImageData()[j + 1];
        watermarkPixels[4 * i + 2] = watermark->GetImageData()[j + 2];
        if (isrgba)
        {
            watermarkPixels[4 * i + 3] = watermark->GetImageData()[j + 3];
            j += 4;
        }
        else
        {
            watermarkPixels[4 * i + 3] = 1;
            j += 3;
        }
    }

    originalImagePixels.resize(4 * originalImage->GetWidth() * originalImage->GetHeight());
    isrgba = originalImage->GetNrChannels() == 4;
    for (int i = 0, j = 0; i < originalImage->GetWidth() * originalImage->GetHeight(); i++)
    {
        originalImagePixels[4 * i] = originalImage->GetImageData()[j];
        originalImagePixels[4 * i + 1] = originalImage->GetImageData()[j + 1];
        originalImagePixels[4 * i + 2] = originalImage->GetImageData()[j + 2];
        if (isrgba)
        {
            originalImagePixels[4 * i + 3] = originalImage->GetImageData()[j + 3];
            j += 4;
        }
        else
        {
            originalImagePixels[4 * i + 3] = 1;
            j += 3;
        }
    }

    float aspect = (float)originalImage->GetWidth() / originalImage->GetHeight();
    window->SetSize(600 * aspect, 600);
    glViewport(0, 0, 600 * aspect, 600);
}

void Tema2::CreateFramebuffer(unsigned int& framebuffer, unsigned int& color_texure, int width, int height)
{
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // Generate and bind the color texture
    glGenTextures(1, &color_texure);
    glBindTexture(GL_TEXTURE_2D, color_texure);
    // Initialize the color textures
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texure, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void m2::Tema2::DoProccesing()
{
    // image borders
    auto shader = shaders["ImageProcessing"];
    shader->Use();
    glViewport(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
        glUniform2i(shader->GetUniformLocation("screenSize"), originalImage->GetWidth(), originalImage->GetHeight());
        glUniform1i(shader->GetUniformLocation("textureImage"), 0);
        originalImage->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }
    imageBPixels.resize(4 * originalImage->GetWidth() * originalImage->GetHeight());
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, imageBPixels.data());
    // watermark borders
    glViewport(0, 0, watermark->GetWidth(), watermark->GetHeight());
    {
        glBindFramebuffer(GL_FRAMEBUFFER, watermark_framebuffer);
        glUniform2i(shader->GetUniformLocation("screenSize"), watermark->GetWidth(), watermark->GetHeight());
        glUniform1i(shader->GetUniformLocation("textureImage"), 0);
        watermark->BindToTextureUnit(GL_TEXTURE0);
        RenderMesh(meshes["quad"], shader, glm::mat4(1));
    }
    watermarkBPixels.resize(4 * watermark->GetWidth() * watermark->GetHeight());
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, watermark->GetWidth(), watermark->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, watermarkBPixels.data());
    firstWhitePixel = glm::vec2(-1, -1);
    lastWhitePixel = glm::vec2(-1, -1);
    for (int y = 0; y < watermark->GetHeight(); y++)
    {
        for (int x = 0; x < watermark->GetWidth(); x++)
        {
			int index = 4 * (y * watermark->GetWidth() + x);
            if (watermarkBPixels[index] == 255)
            {
                if (firstWhitePixel.x == -1)
                {
                    firstWhitePixel = glm::vec2(x, y);
				}
                else
                {
                    lastWhitePixel = glm::vec2(x, y);
                }
			}

		}
    }
    // image compute
    glViewport(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
    concurrency::parallel_for<size_t>(0, originalImage->GetHeight() - watermark->GetHeight() + 1, [&](size_t y)
        {
            for (int x = 0; x < originalImage->GetWidth() - watermark->GetWidth() + 1; x++)
            {
                int first = 4 * ((y + firstWhitePixel.y) * originalImage->GetWidth() + x + firstWhitePixel.x);
                int last = 4 * ((y + lastWhitePixel.y) * originalImage->GetWidth() + x + lastWhitePixel.x);
                if (imageBPixels[first] != 255 && imageBPixels[last] != 255)
                {
                    continue;
				}
                int matchingPixels = 0;
                for (int wy = 0; wy < watermark->GetHeight(); wy++)
                {
                    for (int wx = 0; wx < watermark->GetWidth(); wx++)
                    {
                        int index = 4 * ((y + wy) * originalImage->GetWidth() + x + wx);
                        int watermarkIndex = 4 * (wy * watermark->GetWidth() + wx);
                        if (watermarkBPixels[watermarkIndex] == 255 && imageBPixels[index] == 255)
                        {
                            matchingPixels++;
                            if (matchingPixels > 3000)
                            {
                                goto process;
                            }
                        }
                    }
                }
                process:
                if (matchingPixels > 3000)
                {
                    for (int wy = 0; wy < watermark->GetHeight(); wy++)
                    {
                        for (int wx = 0; wx < watermark->GetWidth(); wx++)
                        {
                            int index = 4 * ((y + wy) * originalImage->GetWidth() + x + wx);
                            int watermarkIndex = 4 * (wy * watermark->GetWidth() + wx);
                            originalImagePixels[index] = max(originalImagePixels[index] - watermarkPixels[watermarkIndex], 0);
                            originalImagePixels[index + 1] = max(originalImagePixels[index + 1] - watermarkPixels[watermarkIndex + 1], 0);
                            originalImagePixels[index + 2] = max(originalImagePixels[index + 2] - watermarkPixels[watermarkIndex + 2], 0);
                        }
                    }
                    x += watermark->GetWidth();
                }
            }
        });
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    processed = true;
}


void Tema2::FrameStart()
{
}


void Tema2::Update(float deltaTimeSeconds)
{
    ClearScreen();

    if (saveScreenToImage)
    {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
        glViewport(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
    }

    auto shader = shaders["DisplayImage"];
    shader->Use();
    glUniform1i(shader->GetUniformLocation("flipVertical"), saveScreenToImage ? 0 : 1);

    GLuint newTexture;
    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, originalImage->GetWidth(), originalImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, originalImagePixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shader->GetUniformLocation("textureImage"), 0);
    RenderMesh(meshes["quad"], shader, glm::mat4(1));

    if (saveScreenToImage)
    {
        saveScreenToImage = false;

        GLenum format = GL_RGB;
        if (originalImage->GetNrChannels() == 4)
        {
            format = GL_RGBA;
        }

        glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
        processedImage->UploadNewData(processedImage->GetImageData());
        SaveImage("shader_processing");

        float aspect = (float)originalImage->GetWidth() / originalImage->GetHeight();
        window->SetSize(600 * aspect, 600);
        glViewport(0, 0, 600 * aspect, 600);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const std::string &fileName)
{
    if (fileName.size())
    {
        std::cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        float aspect = (float)originalImage->GetWidth() / originalImage->GetHeight();
        window->SetSize(600 * aspect, 600);
        glViewport(0, 0, 600 * aspect, 600);

        processed = false;
        CreateFramebuffer(framebuffer_object, color_texture, originalImage->GetWidth(), originalImage->GetHeight());
        originalImagePixels.resize(4 * originalImage->GetWidth() * originalImage->GetHeight());
        bool isrgba = originalImage->GetNrChannels() == 4;
        for (int i = 0, j = 0; i < originalImage->GetWidth() * originalImage->GetHeight(); i++)
        {
            originalImagePixels[4 * i] = originalImage->GetImageData()[j];
            originalImagePixels[4 * i + 1] = originalImage->GetImageData()[j + 1];
            originalImagePixels[4 * i + 2] = originalImage->GetImageData()[j + 2];
            if (isrgba)
            {
                originalImagePixels[4 * i + 3] = originalImage->GetImageData()[j + 3];
                j += 4;
            }
            else
            {
                originalImagePixels[4 * i + 3] = 1;
                j += 3;
            }
        }
    }
}


void Tema2::SaveImage(const std::string &fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
    std::vector<std::string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        std::cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
    {
        saveScreenToImage = true;
    }

    if (key == GLFW_KEY_P)
    {
        DoProccesing();
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
