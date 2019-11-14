#include "MaterialImporter.h"
#include "Application.h"

// Devil ---------------------------------------------------------
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")
//--------------------------------------------------------------------


MaterialImporter::MaterialImporter()
{
	LOG("Initializing DevIL...");
	ilInit();
}


MaterialImporter::~MaterialImporter()
{
}

bool MaterialImporter::Import(const char * path, uint64_t * const uid)
{
	bool ret = false;

	uint64_t id = 0u;

	if (ilLoadImage(path)) {
		std::string mat_path;
		if (App->dummy_file_system->IsInSubDirectory(TEXTURE_A_FOLDER, App->dummy_file_system->GetFileFromPath(path).c_str(), &mat_path)) {
			if (App->dummy_file_system->Exists((mat_path + ".meta").c_str())) {
				if (App->dummy_file_system->IsMetaVaild((mat_path + ".meta").c_str())) {
					id = App->dummy_file_system->GetMeta((mat_path + ".meta").c_str());
					return false;
				}
				else {
					LOG("Meta %s not vaild, recreating meta...", path);
				}
			}
		}
		else {
			mat_path.assign((TEXTURE_A_FOLDER + App->dummy_file_system->GetFileFromPath(path)));
			if (!CopyFile(path, mat_path.data(), FALSE)) {
				LOG("Failed to copy fbx in Assets folder, Error: %s", GetLastError());
			}
		}

		id = App->dummy_file_system->GenerateMetaFile((mat_path + ".meta").c_str());

		ILuint size;
		if (App->dummy_file_system->GetFormat(mat_path.c_str()) != FileSystem::Format::DDS) {
			ILubyte *data = nullptr;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (size > 0) {
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
					if (App->dummy_file_system->Exists(MATERIAL_L_FOLDER) == false)
						App->dummy_file_system->CreateDir(MATERIAL_L_FOLDER);
					ret = App->dummy_file_system->SaveData(data, size, (std::string(MATERIAL_L_FOLDER + std::to_string(id) + ".dds").c_str()));
				}
				RELEASE_ARRAY(data);
			}
		}
		else {
			if (App->dummy_file_system->Exists(MATERIAL_L_FOLDER) == false)
				App->dummy_file_system->CreateDir(MATERIAL_L_FOLDER);
			if (CopyFile(path, (MATERIAL_L_FOLDER + App->dummy_file_system->GetFileFromPath(path)).c_str(), FALSE)) {
				rename((MATERIAL_L_FOLDER + App->dummy_file_system->GetFileFromPath(path)).c_str(), (MATERIAL_L_FOLDER + std::to_string(id) + ".dds").c_str());
			}
			else {
				LOG("Failed to copy fbx in Assets folder, Error: %s", GetLastError());
			}
		}
		if (uid != nullptr)
			*uid = id;
	}
	else {
		LOG("Cannot open image with path %s, Error: %s", path, iluErrorString(ilGetError()));
	}
	
	return ret;
}

Texture* MaterialImporter::Load(const char * path)
{
	Texture* ret = nullptr;

	uint64_t uid = std::stoull(App->dummy_file_system->GetFileNameFromPath(path));
	std::vector<Texture*>* vtex = App->object_manager->GetTextures();
	for (auto i = vtex->begin(); i != vtex->end(); i++) {
		if ((*i)->uid == uid)
			if (App->dummy_file_system->GetFormat(path) == App->dummy_file_system->GetFormat((*i)->path.data())) {
				LOG("Texture already loaded, returning the texture already loaded...");
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
		ret = new Texture(ilutGLBindTexImage(), path, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), uid);

		// Upload pixels into texture
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glBindTexture(GL_TEXTURE_2D, ret->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		App->object_manager->AddTexture(ret);
		LOG("Loaded successfully texture: %s", path);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	ilDeleteImages(1, &devilID);
	return ret;
}
