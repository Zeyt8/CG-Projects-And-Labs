#include "Tema3.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"

using namespace p3;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	camera = new Camera();
	camera->SetPosition(glm::vec3(0, 2, 3.5f));
	camera->RotateThirdPerson_OX(RADIANS(-30));
	gameObjects.push_back(camera);

	player = new Player(this);
	gameObjects.push_back(player);

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Awake();
	}
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Start();
	}

	for (int i = 0; i < objectsToAdd.size(); i++)
	{
		objectsToAdd[i]->Awake();
		objectsToAdd[i]->Start();
		gameObjects.push_back(objectsToAdd[i]);
	}
	objectsToAdd.clear();

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	{
		Shader* shader = new Shader("Texture");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "textureVS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "textureFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("Snow");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "snowVS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "snowFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema3::AddObject(GameObject* object)
{
	objectsToAdd.push_back(object);
}

void Tema3::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.23f, 0.71f, 0.87f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Tema3::Update(float deltaTimeSeconds)
{
	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		if (gameObjects[i]->Destroy)
		{
			gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObjects[i]), gameObjects.end());
			delete gameObjects[i];
			continue;
		}
		gameObjects[i]->Update(deltaTimeSeconds);
		gameObjects[i]->Render();
	}
	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		gameObjects[i]->LateUpdate(deltaTimeSeconds);
	}
	for (int i = 0; i < objectsToAdd.size(); i++)
	{
		objectsToAdd[i]->Awake();
		objectsToAdd[i]->Start();
		gameObjects.push_back(objectsToAdd[i]);
	}
	objectsToAdd.clear();
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture)
	{
		// - activate texture location 0
		// - bind the texture ID
		// - send the uniform value
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::FrameEnd()
{
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
	for (GameObject* object : gameObjects)
	{
		object->OnInputUpdate(deltaTime, mods);
	}
}

void Tema3::OnKeyPress(int key, int mods)
{
}

void Tema3::OnKeyRelease(int key, int mods)
{
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
