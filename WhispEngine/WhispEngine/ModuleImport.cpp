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
		
		GameObject * container = App->object_manager->CreateGameObject(nullptr);
		container->SetName(App->file_system->GetFileNameFromPath(path).data());
		aiNode *node = scene->mRootNode;
		LoadNode(node, container, scene);

		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene: %s", aiGetErrorString());

	return ret;
}

void ModuleImport::LoadNode(aiNode * node, GameObject * parent, const aiScene * scene)
{
	for (int i = 0; i < node->mNumChildren; ++i) {
		aiNode* child = node->mChildren[i];

		GameObject* obj = App->object_manager->CreateGameObject(parent);
		obj->SetName(child->mName.C_Str());

		if (child->mNumMeshes == 1) {
			ComponentMesh* mesh = static_cast<ComponentMesh*>(obj->CreateComponent(ComponentType::MESH));
			aiMesh* amesh = scene->mMeshes[child->mMeshes[0]];
			mesh->mesh = App->object_manager->CreateMesh(amesh);

			aiMaterial* aimaterial = scene->mMaterials[amesh->mMaterialIndex];
			aiString path;
			aimaterial->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
			ImportTexture(std::string(std::string("Assets/Textures/") + path.C_Str()).data());
		}
		else {
			for (int j = 0; j < child->mNumMeshes; ++j) {
				GameObject * child_m = App->object_manager->CreateGameObject(obj);

				ComponentMesh* mesh = static_cast<ComponentMesh*>(child_m->CreateComponent(ComponentType::MESH));
				aiMesh* amesh = scene->mMeshes[child->mMeshes[j]];
				mesh->mesh = App->object_manager->CreateMesh(amesh);

				child_m->SetName(amesh->mName.C_Str());
			}
		}

		if (child->mNumChildren > 0) {
			LoadNode(child, obj, scene);
		}
	}
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
