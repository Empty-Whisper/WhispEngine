#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL/include/SDL_opengl.h"
#include "PanelScene.h"
#include "PanelGame.h"
#include "Brofiler/Brofiler.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name.assign("Renderer");

	scene_viewport = CreateViewport();
	game_viewport = CreateViewport();

}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(nlohmann::json &node)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);

	vsync = node["vsync"];
	App->framerate_cap = node["framerate_cap"];
	
	GLenum init = glewInit();
	if (init != GLEW_OK) {
		LOG("Fail to init glew: %s", (const char*)glewGetErrorString(init));
		ret = false;
	}

	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if (vsync && SDL_GL_SetSwapInterval(1) < 0)
		{
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}
	

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", glewGetErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		background_color = new float[3];
		background_color[0] = 0.109f;
		background_color[1] = 0.109f;
		background_color[2] = 0.109f;
		glClearColor(background_color[0], background_color[1], background_color[2], 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		glEnable(GL_NORMALIZE);
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		
		if (fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if(wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Attributes --------------------------------------------------
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}

	// Projection matrix for
	InitTextureBuffers(scene_viewport);
	InitTextureBuffers(game_viewport);
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	BROFILER_CATEGORY("Render", Profiler::Color::FireBrick);
	/*GetSceneViewport()->SetMatrix(App->camera->GetSceneCamera());
	GetGameViewport()->SetMatrix(App->camera->GetGameCamera());*/

	Camera* cam = nullptr;
#ifndef GAME_BUILD
	cam = App->camera->GetSceneCamera();
#else
	cam = App->camera->GetGameCamera();
#endif
	if (cam != nullptr)
		lights[0].SetPos(cam->GetFrustum().pos.x, cam->GetFrustum().pos.y, cam->GetFrustum().pos.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update()
{
	BROFILER_CATEGORY("Render", Profiler::Color::FireBrick);

#ifndef GAME_BUILD
	if (!is_rendering_scene) // TODO: Threads ;)
	{
		// SCENE ============================================================================================ =
		GetSceneViewport()->SetMatrix(App->camera->GetSceneCamera());
		GetSceneViewport()->UpdateBind(App->renderer3D->scene_viewport->render_texture);

		// DockSpace
		if (scene_viewport->can_resize)
		{
			ResizeDockspace(App->gui->scene->GetPanelSize(), scene_viewport, App->camera->GetSceneCamera());
			scene_viewport->can_resize = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(App->camera->GetSceneCamera()->GetViewMatrix().ptr());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(background_color[0], background_color[1], background_color[2], 1.f);

		is_rendering_scene = true;
	}
	else
	{
#endif
		// Game =============================================================================================
		GetGameViewport()->SetMatrix(App->camera->GetGameCamera());
		GetGameViewport()->UpdateBind(App->renderer3D->game_viewport->render_texture);

		//Dockspace
		if (game_viewport->can_resize)
		{
			ResizeDockspace(App->gui->game->GetPanelSize(), game_viewport, App->camera->GetGameCamera());
			game_viewport->can_resize = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(App->camera->GetGameCamera()->GetViewMatrix().ptr());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(background_color[0], background_color[1], background_color[2], 1.f);
#ifndef GAME_BUILD
		is_rendering_scene = false;
	}
#endif

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate()
{
	BROFILER_CATEGORY("Render", Profiler::Color::FireBrick);

	SDL_GL_SwapWindow(App->window->window);

	//// DockSpace
	//if (scene_viewport->can_resize)
	//{
	//	ResizeDockspace(App->gui->scene->GetPanelSize(), scene_viewport);
	//	scene_viewport->can_resize = false;
	//}
	//
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glLoadIdentity();

	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(App->camera->GetSceneCamera()->GetViewMatrix().ptr());

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glClearColor(background_color[0], background_color[1], background_color[2], 1.f);

	/*if (game_viewport->can_resize)
	{
		ResizeDockspace(App->gui->game->GetPanelSize(), game_viewport);
		game_viewport->can_resize = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetGameCamera()->GetViewMatrix().ptr());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(background_color[0], background_color[1], background_color[2], 1.f);*/
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);
	DeleteViewport(game_viewport);
	DeleteViewport(scene_viewport);
	delete[] background_color;

	return true;
}

bool ModuleRenderer3D::Save(nlohmann::json & node) const
{
	node["vsync"] = vsync;
	node["framerate_cap"] = App->framerate_cap;

	return true;
}


bool ModuleRenderer3D::Load(nlohmann::json & node)
{
	vsync = node["vsync"];
	App->framerate_cap = node["framerate_cap"];

	//SDL_GL_SetSwapInterval(vsync);

	return true;
}

Viewport * ModuleRenderer3D::CreateViewport()
{
	return new Viewport();
}

void ModuleRenderer3D::DeleteViewport(Viewport * viewport)
{
	RELEASE(viewport);
}


Viewport * ModuleRenderer3D::GetSceneViewport()
{
	return scene_viewport;
}

Viewport * ModuleRenderer3D::GetGameViewport()
{
	return game_viewport;
}

void ModuleRenderer3D::SetSceneViewport(Viewport * viewport)
{
	if (viewport != nullptr)
		scene_viewport = viewport;

}

void ModuleRenderer3D::SetGameViewport(Viewport * viewport)
{
	if (viewport != nullptr)
		game_viewport = viewport;
}

const bool ModuleRenderer3D::CanResize(Viewport * viewport)
{
	return viewport->can_resize = true;
}

void ModuleRenderer3D::ResizeDockspace(ImVec2 size, Viewport* viewport, Camera* camera)
{
	glViewport(0, 0, size.x, size.y);

	glMatrixMode(GL_PROJECTION);
	//ProjectionMatrix = perspective(60.0f, (float)size.x / (float)size.y, 0.125f, 512.0f);
	//glLoadMatrixf(&ProjectionMatrix);
	glLoadMatrixf((GLfloat*)&camera->GetFrustum().ProjectionMatrix());

	//TODO: Poner el projection matrix del frustum
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	UpdateTextureBuffers(size.x, size.y, viewport);
}

void ModuleRenderer3D::OnResize(int width, int height, Camera* camera)
{
	camera->SetAspectRatio((float)width / (float)height);

	//glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf(camera->GetProjectionMatrix().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::InitTextureBuffers(Viewport* viewport)
{
	glGenRenderbuffers(1, &viewport->depth_render_buffer);
	glGenTextures(1, &viewport->render_texture);
	glGenFramebuffers(1, &viewport->frame_buffer);

}

void ModuleRenderer3D::UpdateTextureBuffers(int width, int height, Viewport* viewport)
{
	// Depth
	glBindRenderbuffer(GL_RENDERBUFFER, viewport->depth_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Texture
	glBindTexture(GL_TEXTURE_2D, viewport->render_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Depth and Texture to Frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, viewport->frame_buffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, viewport->depth_render_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewport->render_texture, 0);

	// If program can generate the texture 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("[Error] creating screen buffer");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Viewport::Viewport()
{
}

void Viewport::SetMatrix(Camera * camera)
{
	//Set ProjectionMatrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(camera->GetProjectionMatrix().ptr());

	//Set ViewMatrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetViewMatrix().ptr());
}

void Viewport::UpdateBind(uint texture_id)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
