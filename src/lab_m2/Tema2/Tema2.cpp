#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>

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

    float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
    CreateFramebuffer(framebuffer_object, color_texture, static_cast<int>(600 * aspectRatio), 600);
    CreateFramebuffer(watermark_framebuffer, watermark_color_texture, watermark->GetWidth(), watermark->GetHeight());
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texure, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    }

    float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();

    auto shader = shaders["DisplayImage"];
    shader->Use();

    glUniform1i(shader->GetUniformLocation("flipVertical"), saveScreenToImage ? 1 : 0);
    if (processed)
    {
        GLuint newTexture;
        glGenTextures(1, &newTexture);
        glBindTexture(GL_TEXTURE_2D, newTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, originalImage->GetWidth(), originalImage->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        originalImage->BindToTextureUnit(GL_TEXTURE0);
    }
    
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
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
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

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
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
        processed = false;
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
        auto shader = shaders["ImageProcessing"];
        shader->Use();
        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        int width = static_cast<int>(600 * aspectRatio);

        pixels.resize(4 * width * 600);
        for (int i = 0, j = 0; i < width * 600; i++)
        {
			pixels[4 * i] = processedImage->GetImageData()[j];
            pixels[4 * i + 1] = processedImage->GetImageData()[j + 1];
            pixels[4 * i + 2] = processedImage->GetImageData()[j + 2];
            j += 3;
            pixels[4 * i + 3] = 1;
		}

        std::vector<GLubyte> watermarkP(4 * watermark->GetWidth() * watermark->GetHeight());
        for (int i = 0, j = 0; i < watermark->GetWidth() * watermark->GetHeight(); i++)
        {
			watermarkP[4 * i] = watermark->GetImageData()[j];
			watermarkP[4 * i + 1] = watermark->GetImageData()[j + 1];
			watermarkP[4 * i + 2] = watermark->GetImageData()[j + 2];
            j += 3;
			watermarkP[4 * i + 3] = 1;
		}

        {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);

            int screenSize_loc = shader->GetUniformLocation("screenSize");
            glm::ivec2 resolution = window->GetResolution();
            glUniform2i(screenSize_loc, resolution.x, resolution.y);

            int locTexture = shader->GetUniformLocation("textureImage");
            glUniform1i(locTexture, 0);

            originalImage->BindToTextureUnit(GL_TEXTURE0);

            RenderMesh(meshes["quad"], shader, glm::mat4(1));
        }

        std::vector<GLubyte> imagePixels(4 * width * 600);
        glReadPixels(0, 0, width, 600, GL_RGBA, GL_UNSIGNED_BYTE, imagePixels.data());

        {
            glBindFramebuffer(GL_FRAMEBUFFER, watermark_framebuffer);

            glUniform2i(shader->GetUniformLocation("watermarkSize"), watermark->GetWidth(), watermark->GetHeight());

            int locWatermark = shader->GetUniformLocation("textureImage");
            glUniform1i(locWatermark, 0);
            watermark->BindToTextureUnit(GL_TEXTURE0);
            RenderMesh(meshes["quad"], shader, glm::mat4(1));
        }

        std::vector<GLubyte> watermarkPixels(4 * watermark->GetWidth() * watermark->GetHeight());
        glReadPixels(0, 0, watermark->GetWidth(), watermark->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, watermarkPixels.data());

        for (int y = 0; y < 600 - watermark->GetHeight(); y++)
        {
            for (int x = 0; x < width - watermark->GetWidth(); x++)
            {
                int matchingPixels = 0;
                for (int i = 0; i < watermark->GetWidth(); i++)
                {
                    for (int j = 0; j < watermark->GetHeight(); j++)
                    {
                        int index = 4 * ((y + j) * width + x + i);
                        int watermarkIndex = 4 * (j * watermark->GetWidth() + i);
                        if (watermarkPixels[watermarkIndex] == imagePixels[index])
                        {
                            matchingPixels++;
                        }
                    }
                }
                if (matchingPixels > 0.99f * watermark->GetWidth() * watermark->GetHeight())
                {
                    for (int i = 0; i < watermark->GetWidth(); i++)
                    {
                        for (int j = 0; j < watermark->GetHeight(); j++)
                        {
                            int index = 4 * ((y + j) * width + x + i);
                            int watermarkIndex = 4 * (j * watermark->GetWidth() + i);
                            pixels[index] -= watermarkP[watermarkIndex];
                            pixels[index + 1] -= watermarkP[watermarkIndex + 1];
                            pixels[index + 2] -= watermarkP[watermarkIndex + 2];
                            pixels[index + 3] -= watermarkP[watermarkIndex + 3];
                        }
                    }
                    x += watermark->GetWidth();
                }
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        processed = true;
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
