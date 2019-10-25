#include "ModuleImport.h"

#include "Application.h"
#include "ModuleObjectManager.h"

#include "ComponentTransform.h"

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

	// Charge logo texture
	logo_txt = ImportTexture("Assets/logo.png");
	logo_txt->visible_on_inspector = false;

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

	LOG("Importing fbx with path: %s", path);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	if (scene != nullptr && scene->HasMeshes())
	{
		
		GameObject * container = App->object_manager->CreateGameObject(nullptr);
		container->SetName(App->file_system->GetFileNameFromPath(path).data());
		
		aiNode *node = scene->mRootNode;

		/*aiVector3D position, scale;			// This will be commented only for assignment 1
		aiQuaternion rotation;
		node->mTransformation.Decompose(scale, rotation, position);
		ComponentTransform* transform = ((ComponentTransform*)container->GetComponent(ComponentType::TRANSFORM));
		transform->SetPosition(position.x, position.y, position.z);
		transform->SetRotation(rotation.w, rotation.x, rotation.y, rotation.z);
		transform->SetScale(scale.x, scale.y, scale.z);

		transform->CalculeLocalMatrix();
		transform->CalculateGlobalMatrix();*/

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
		LOG("Created %s GameObject", obj->GetName());

		ComponentTransform *transform = (ComponentTransform*)obj->GetComponent(ComponentType::TRANSFORM);
		aiVector3D position, scale;
		aiQuaternion rotation;
		child->mTransformation.Decompose(scale, rotation, position);

		transform->SetPosition(position.x, position.y, position.z);
		transform->SetRotation(rotation.w, rotation.x, rotation.y, rotation.z);
		// FBX exporters have some options that will change the scale of the models, be sure you export your models in Apply Scale FBX All mode

		//scale *= 0.01f;
		//scale /= std::max(std::max(scale.x, scale.y),scale.z); 
		transform->SetScale(scale.x, scale.y, scale.z);

		transform->CalculeLocalMatrix();
		transform->CalculateGlobalMatrix();

		if (child->mNumMeshes == 1) {
			ComponentMesh* mesh = (ComponentMesh*)obj->CreateComponent(ComponentType::MESH);
			aiMesh* amesh = scene->mMeshes[child->mMeshes[0]];
			mesh->mesh = App->object_manager->CreateMesh(amesh);

			aiMaterial* aimaterial = scene->mMaterials[amesh->mMaterialIndex];
			aiString path;
			aimaterial->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
			LOG("Diffuse texture found: %s", path.C_Str());
			ComponentMaterial* material = (ComponentMaterial*)obj->GetComponent(ComponentType::MATERIAL);
			material->SetTexture(ImportTexture(std::string(std::string("Assets/Textures/") + App->file_system->GetFileFromPath(path.C_Str())).data()));

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

Texture* ModuleImport::ImportTexture(const char * path)
{
	Texture* ret = nullptr;

	std::string tmp = App->file_system->GetFileNameFromPath(path); // Lazy way to compare if the texture already exists, will work for now TODO: improve it (std::filesystem?)
	std::vector<Texture*>* vtex = App->object_manager->GetTextures();
	for (auto i = vtex->begin(); i != vtex->end(); i++) {
		if (tmp.compare((*i)->name) == 0)
			if (App->file_system->GetFormat(path) == App->file_system->GetFormat((*i)->path.data())) {
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

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	ilDeleteImages(1, &devilID);

	return ret;
}
