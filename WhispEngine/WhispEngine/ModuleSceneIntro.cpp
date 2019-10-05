#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"

#include "Imgui/imgui.h"
#include "GameObject.h"

//Assimp--------------------------------
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

//----------------------------------------------------

//MathGeoLib--------------------------------------------------------
#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/lib_x86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "MathGeoLib/lib_x86/Release/MathGeoLib.lib")
#endif
//--------------------------------------------------------------------


ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	name.assign("SceneIntro");
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	
	App->camera->Move(vec3(5.0f, 3.0f, 5.0f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	
	/*par_shapes_mesh *m = par_shapes_create_plane(3, 3);
	cube = new GameObject(m->npoints, m->points, m->ntriangles, (uint*)m->triangles);
	par_shapes_free_mesh(m);*/

	// Stream log messages to Debug window
	//struct aiLogStream stream;
	//stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	//aiAttachLogStream(&stream);

	//const aiScene* scene = aiImportFile("warrior.FBX", aiProcessPreset_TargetRealtime_MaxQuality);

	//w = new GameObject();

	//if (scene != nullptr && scene->HasMeshes())
	//{
	//	// Use scene->mNumMeshes to iterate on scene->mMeshes array
	//	/*uint n_v = 0;
	//	uint n_i = 0;
	//	float* v = nullptr;
	//	uint* a_i = nullptr;*/

	//	aiMesh* it = nullptr;
	//	for (uint i = 0; i < scene->mNumMeshes; ++i) 
	//	{
	//		it = scene->mMeshes[i];
	//		w->n_vertex = it->mNumVertices;
	//		w->vertex = new float[w->n_vertex * 3];
	//		memcpy(w->vertex, it->mVertices, sizeof(float) * w->n_vertex * 3);
	//		LOG("New mesh with %d", w->vertex);

	//		if (it->HasFaces()) 
	//		{
	//			w->n_index = it->mNumFaces * 3;
	//			w->index = new uint[w->n_index];

	//			for (uint j = 0; j < it->mNumFaces; ++j) 
	//			{
	//				if (it->mFaces[j].mNumIndices != 3) 
	//				{
	//					LOG("WARNING, geometry face with != 3 indices!");
	//				}
	//				else 
	//				{
	//					memcpy(&w->index[j * 3], it->mFaces[j].mIndices, sizeof(uint) * 3);
	//				}
	//			}
	//		}

	//		//warrior = new GameObject(n_v, v, n_i, a_i);
	//		w->SetGLBuffers();
	//		/*delete[] v;
	//		v = nullptr;
	//		delete[] a_i;
	//		a_i = nullptr;*/

	//		it = nullptr;
	//	}

	//	aiReleaseImport(scene);
	//}
	//else
	//	LOG("Error loading scene: %s", aiGetErrorString());
	
	return ret;
}

// Update
update_status ModuleSceneIntro::Update()
{
	glColor3f(1.f, 0.f, 0.f);
	// activate and specify pointer to vertex array

	glEnableClientState(GL_VERTEX_ARRAY);
	
	//cube->Draw();
	//w->Draw();

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	DrawGrid(50);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawGrid(int width)
{
	glBegin(GL_LINES);
	for (int i = -width; i <= width; ++i)
	{

		glVertex3f(i, 0, -width);
		glVertex3f(i, 0, width);

		glVertex3f(width, 0, i);
		glVertex3f(-width, 0, i);

		glColor3f(1.f, 1.f, 1.f);

	}
	glEnd();
}


// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	
	delete cube;
	delete w;

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}