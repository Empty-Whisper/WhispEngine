#include "ModuleImport.h"

#include "Application.h"
#include "ModuleObjectManager.h"

#include "ComponentTransform.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Brofiler/Brofiler.h"

#include "ModelImporter.h"
#include "MaterialImporter.h"
#include "MeshImporter.h"

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

	LOG("Initializing DevIL...");
	ilInit();
	
	model		= new ModelImporter();
	material	= new MaterialImporter();
	mesh		= new MeshImporter();

	// Charge logo texture
	logo_txt = ImportTexture("Assets/logo.png");
	logo_txt->visible_on_inspector = false;

	return true;
}

bool ModuleImport::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	delete model;
	delete material;
	delete mesh;

	return true;
}

bool ModuleImport::Import(const char * path)
{
	bool ret = false;
	switch (App->dummy_file_system->GetFormat(path))
	{
	case FileSystem::Format::JSON:
		break;
	case FileSystem::Format::DDS:
	case FileSystem::Format::PNG:
	case FileSystem::Format::JPG:
		ret = material->Import(path);
		break;
	case FileSystem::Format::FBX:
		ret = model->Import(path);
		break;
	case FileSystem::Format::MODEL:
		ret = model->Load(path);
		break;
	case FileSystem::Format::META: {
		char* f_uid = App->dummy_file_system->GetData(path);
		uint64_t uid = 0u;
		memcpy(&uid, f_uid, sizeof(uint64_t));

		delete[] f_uid;

		if (uid != 0u) {
			ret = model->Load((MODEL_L_FOLDER + std::to_string(uid) + ".whispModel").c_str());
		}
	}
		break;
	default:
		LOG("Failed to load %s. Format not setted");
		break;
	}
	return ret;
}

Texture* ModuleImport::ImportTexture(const char * path)
{
	Texture* ret = nullptr;

	std::string tmp = App->dummy_file_system->GetFileNameFromPath(path); // Lazy way to compare if the texture already exists, will work for now TODO: improve it (std::filesystem?)
	std::vector<Texture*>* vtex = App->object_manager->GetTextures();
	for (auto i = vtex->begin(); i != vtex->end(); i++) {
		if (tmp.compare((*i)->name) == 0)
			if (App->dummy_file_system->GetFormat(path) == App->dummy_file_system->GetFormat((*i)->path.data())) {
				LOG("Texture already loaded, returning the texture already loaded...");
				if (App->object_manager->GetSelected() != nullptr) {// Assign new texture to object selected (ONLY FOR THE FIRST ASSIGNMENT) TODO: Delete this after first delivery
					ComponentMaterial* mat = (ComponentMaterial*)App->object_manager->GetSelected()->GetComponent(ComponentType::MATERIAL);
					if (mat != nullptr)	mat->SetTexture(*i);
				}
				return *i;
			}
	}

	ILuint devilID = 0;

	ilGenImages(1, &devilID);
	ilBindImage(devilID);

	ilutRenderer(ILUT_OPENGL);  // Switch the renderer

	if (!ilLoadImage(path)) {
		auto error = ilGetError();
		LOG("Failed to load texture with path: %s. Error: %s", path, ilGetString(error));
	}
	else {
		ret = new Texture(ilutGLBindTexImage(), path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, ret->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (App->object_manager->GetSelected() != nullptr) {// Assign new texture to object selected (ONLY FOR THE FIRST ASSIGNMENT) TODO: Delete this after first delivery
			ComponentMaterial* mat = (ComponentMaterial*)App->object_manager->GetSelected()->GetComponent(ComponentType::MATERIAL);
			if (mat != nullptr)	mat->SetTexture(ret);
		}
		App->object_manager->AddTexture(ret);
		LOG("Loaded successfully texture: %s", path);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	ilDeleteImages(1, &devilID);

	return ret;
}
