#include "Tema3.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Map.h"
#include "ObjectSpawner.h"
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

using namespace p3;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	_camera = new Camera();
	_camera->SetPosition(glm::vec3(0, 2, 3.5f));
	_camera->RotateThirdPerson_OX(RADIANS(-20));
	_camera->projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	GameObjects.push_back(_camera);

	PlayerObject = new Player(this);
	GameObjects.push_back(PlayerObject);

	_camera->followTarget = PlayerObject;

	Map* map = new Map(this);
	GameObjects.push_back(map);

	ObjectSpawner* objectSpawner = new ObjectSpawner(this);
	GameObjects.push_back(objectSpawner);

	for (int i = 0; i < GameObjects.size(); i++)
	{
		GameObjects[i]->Awake();
	}
	for (int i = 0; i < GameObjects.size(); i++)
	{
		GameObjects[i]->Start();
	}

	for (int i = 0; i < _objectsToAdd.size(); i++)
	{
		_objectsToAdd[i]->Awake();
		_objectsToAdd[i]->Start();
		GameObjects.push_back(_objectsToAdd[i]);
	}
	_objectsToAdd.clear();

	{
		Shader* shader = new Shader("texture");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "textureVS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "textureFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("snow");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "snowVS.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "snowFS.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Tema3::AddObject(GameObject* object)
{
	_objectsToAdd.push_back(object);
}

glm::vec3 Tema3::ConvertToTiltedPlane(glm::vec3 vector)
{
	return glm::rotateX(vector, RADIANS(30));
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
	for (int i = GameObjects.size() - 1; i >= 0; i--)
	{
		if (GameObjects[i]->ToDestroy)
		{
			const GameObject* go_to_delete = GameObjects[i];
			GameObjects.erase(GameObjects.begin() + i);
			delete go_to_delete;
			continue;
		}
		GameObjects[i]->Update(deltaTimeSeconds);
		GameObjects[i]->Render();
	}
	for (GameObject* go : GameObjects)
	{
		go->LateUpdate(deltaTimeSeconds);
	}
	for (GameObject* go : GameObjects)
	{
		for (GameObject* other : GameObjects)
		{
			if (go != other)
			{
				if (glm::distance(go->Position, other->Position) < go->ColliderRadius + other->ColliderRadius)
				{
					go->OnCollisionEnter(other);
				}
			}
		}
	}
	for (int i = 0; i < _objectsToAdd.size(); i++)
	{
		_objectsToAdd[i]->Awake();
		_objectsToAdd[i]->Start();
		GameObjects.push_back(_objectsToAdd[i]);
	}
	_objectsToAdd.clear();
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, bool time)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetCamera()->projectionMatrix;
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture)
	{
		// - activate texture location 0
		// - bind the texture ID
		// - send the uniform value
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	if (time)
	{
		glUniform2fv(glGetUniformLocation(shader->program, "displacement"), 1, glm::value_ptr(glm::vec2(PlayerObject->Position.x, PlayerObject->Position.z) / 50.0f));
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
	for (GameObject* object : GameObjects)
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
