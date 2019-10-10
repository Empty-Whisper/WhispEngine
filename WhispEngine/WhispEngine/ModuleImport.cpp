#include "ModuleImport.h"

#include "Application.h"
#include "ModuleObjectManager.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

// Devil ---------------------------------------------------------
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")
//--------------------------------------------------------------------

#pragma comment (lib, "Assimp/libx86/assimp.lib")




ModuleImport::ModuleImport()
{
	name.assign("Importer");
}


ModuleImport::~ModuleImport()
{
}

bool ModuleImport::Start()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();

	return true;
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

bool ModuleImport::ImportFbx(const char * path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject *obj = new GameObject();

		aiMesh* it = nullptr;
		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			it = scene->mMeshes[i];
			Mesh* mesh = App->object_manager->CreateMesh(it);
			obj->mesh.push_back(mesh);
			it = nullptr;
		}
		App->object_manager->AddObject(obj);

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene: %s", aiGetErrorString());

	return ret;
}

bool ModuleImport::ImportTexture(const char * path)
{
	bool ret = true;

	ILuint devilID = 0;

	ilGenImages(1, &devilID);
	ilBindImage(devilID);

	ilutRenderer(ILUT_OPENGL);  // Switch the renderer

	if (!ilLoad(IL_DDS, path)) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
		ret = false;
	}
	else {
		Texture texture(ilutGLBindTexImage(), path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		App->object_manager->AddTexture(texture);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	ilDeleteImages(1, &devilID);

	return ret;
}
