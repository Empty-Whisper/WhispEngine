#include "MaterialImporter.h"
#include "Application.h"
#include "ModuleObjectManager.h"

#include "ModuleResources.h"
#include "ResourceTexture.h"

#include "glew-2.1.0/include/GL/glew.h"
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

uint64 MaterialImporter::Import(const char * path, const aiMaterial * material, const uint64 & force_uid)
{
	uint64 meta = force_uid;
	if (meta == 0u)
		meta = App->random->RandomGUID();
	
	if (material != nullptr) {
		aiString tex_path;
		if (material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, NULL, &tex_path) == aiReturn_FAILURE) {
			LOG("Cannot get texture %s from %s model", tex_path.C_Str(), path);
			return 0U;
		}
		path = tex_path.C_Str();
	}

	std::string file = App->file_system->GetFileFromPath(path);
	std::string mat_path(path);
	if (App->file_system->IsInSubDirectory(ASSETS_FOLDER, file.c_str(), &mat_path)) {
		if (App->file_system->Exists((mat_path + ".meta").c_str())) {
			if (App->file_system->IsMetaVaild((mat_path + ".meta").c_str())) {
				meta = App->file_system->GetUIDFromMeta((mat_path + ".meta").c_str());
				Resource* res = App->resources->CreateResource(Resource::Type::TEXTURE, meta);
				res->SetFile(mat_path.c_str());
				res->SetResourcePath((MATERIAL_L_FOLDER + std::to_string(meta) + ".dds").c_str());
				// TODO: load resource
				
				return res->GetUID();
			}
			else {
				LOG("Meta %s not vaild, recreating...", path);
				meta = App->file_system->GetUIDFromMeta((mat_path + ".meta").c_str());
			}
		}
	}
	else {
		LOG("File not found in Assets folder, trying to find file in fbx folder...")
			mat_path.assign(std::string(TEXTURE_A_FOLDER) + file);
		if (!CopyFile(path, mat_path.data(), FALSE)) {
			wchar_t buf[256];
			FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
			LOG("Failed to copy texture in Assets folder, Error: %s", buf);
			return 0U;
		}
	}

	
	bool success = false;
	if (App->file_system->GetPathFormat(mat_path.c_str()).compare("tga") == 0) {
		success = ilLoad(IL_TGA, mat_path.c_str());
	}
	else
		success = ilLoadImage(mat_path.c_str());

	if (success) {
		ResourceTexture* mat = (ResourceTexture*)App->resources->CreateResource(Resource::Type::TEXTURE, meta);

		App->file_system->GenerateMetaFile(mat_path.c_str(), mat->GetUID());

		mat->SetFile(mat_path.c_str());
		mat->SetResourcePath(std::string(MATERIAL_L_FOLDER + std::to_string(mat->GetUID()) + ".dds").c_str());
		mat->Set(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0);

		ILuint size;
		if (App->file_system->GetFormat(mat_path.c_str()) != FileSystem::Format::DDS) {
			ILubyte *data = nullptr;
			ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
			size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
			if (size > 0) {
				data = new ILubyte[size]; // allocate data buffer
				if (ilSaveL(IL_DDS, data, size) > 0) { // Save to buffer with the ilSaveIL function
					if (App->file_system->Exists(MATERIAL_L_FOLDER) == false)
						App->file_system->CreateDir(MATERIAL_L_FOLDER);
					App->file_system->SaveData(data, size, mat->GetLibraryPath());
				}
				else
					LOG("Failed to generate DDS");
				RELEASE_ARRAY(data);
				return mat->GetUID();
			}
			else
				LOG("Size of image is 0");
		}
		else {
			if (App->file_system->Exists(MATERIAL_L_FOLDER) == false)
				App->file_system->CreateDir(MATERIAL_L_FOLDER);

			if (CopyFile(mat_path.c_str(), (MATERIAL_L_FOLDER + App->file_system->GetFileFromPath(file.c_str())).c_str(), FALSE)) {
				rename((MATERIAL_L_FOLDER + App->file_system->GetFileFromPath(file.c_str())).c_str(), mat->GetLibraryPath());
				return mat->GetUID();
			}
			else {
				LOG("Failed to copy material in Library folder, Cannot copy %s in %s", mat_path.c_str(), (MATERIAL_L_FOLDER + App->file_system->GetFileFromPath(file.c_str())).c_str());
			}
		}
	}
	else {
		LOG("Cannot open image with path %s, Error: %i", file, ilGetError());
	}
	return 0u;
}
