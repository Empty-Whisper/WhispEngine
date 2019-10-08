#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(nlohmann::json &node);
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	bool Save(nlohmann::json &node) const;
	bool Load(nlohmann::json &node);

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	GLenum vision_mode = GL_FILL;
	float* background_color = nullptr;

	bool wireframe = true;
	bool fill = true;
	bool see_normals = false;

	bool vsync = true;
};