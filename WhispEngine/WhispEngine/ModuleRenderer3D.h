#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include "ComponentMaterial.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(nlohmann::json &node);
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	update_status Draw();
	bool CleanUp();

	bool Save(nlohmann::json &node) const;
	bool Load(nlohmann::json &node);

	bool CanResize();
	void OnResize(int width, int height);

	void InitTextureBuffers();
	void UpdateTextureBuffers(int width, int height);
public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	GLenum vision_mode = GL_FILL;
	float* background_color = nullptr;

	bool wireframe = false;
	bool fill = true;

	bool vsync = true;

	uint frame_buffer = 0;
	uint render_texture = 0;
	uint depth_render_buffer = 0;
	uint stencil_buffer = 0;

	bool can_resize = false;
};