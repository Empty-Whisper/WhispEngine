#include "FileSystem.h"
#include <fstream>
#include <iomanip>
#include <filesystem>
#include "Application.h"
#include "Globals.h"

FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}

nlohmann::json FileSystem::OpenFile(const char * path)
{
	nlohmann::json file;
	std::ifstream i(path);
	if (i.fail()) {
		LOG("Failed to open %s", path);
		return nullptr;
	}

	i >> file;

	return file;
}

void FileSystem::SaveFile(const char * path, const nlohmann::json & to_save)
{
	std::ofstream o(path);
	o << std::setw(4) << to_save << std::endl;
	o.close();
}

FileSystem::Format FileSystem::GetFormat(const char * file) const
{
	std::string buffer = GetPathFormat(file);

	if (buffer.compare("json") == 0) {
		return FileSystem::Format::JSON;
	}
	else if (buffer.compare("fbx") == 0) {
		return FileSystem::Format::FBX;
	}
	else if (buffer.compare("dds") == 0) {
		return FileSystem::Format::DDS;
	}
	else if (buffer.compare("png") == 0) {
		return FileSystem::Format::PNG;
	}
	else if (buffer.compare("jpg") == 0) {
		return FileSystem::Format::JPG;
	}
	else if (buffer.compare("whispmodel") == 0) {
		return FileSystem::Format::MODEL;
	}
	else if (buffer.compare("meta") == 0) {
		return FileSystem::Format::META;
	}
	else if (buffer.compare("scene") == 0) {
		return FileSystem::Format::SCENE;
	}
	else if (buffer.compare("tga") == 0) {
		return FileSystem::Format::TGA;
	}
	
	LOG("Cannot identify format, format is: %s", buffer.data());

	return FileSystem::Format::NONE;
}

Resource::Type FileSystem::GetResourceType(const char * file) const
{
	switch (GetFormat(file))
	{
	case FileSystem::Format::DDS:
	case FileSystem::Format::JPG:
	case FileSystem::Format::PNG:
		return Resource::Type::TEXTURE;
		break;
	case FileSystem::Format::FBX:
		return Resource::Type::MODEL;
		break;
	default:
		LOG("%s has not any extension that correspond to a resource", file);
		break;
	}
	return Resource::Type::NONE;
}

std::string FileSystem::GetPathFormat(const char* path) const
{
	std::string f(path);
	std::string ret;
	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (*i != '.')
			ret.push_back(std::tolower(*i));
		else
			break;
	}
	std::reverse(ret.begin(), ret.end());

	return ret;
}

std::string FileSystem::GetFileNameFromPath(const char * file) const
{
	std::string f(file);
	std::string buffer;

	bool dot_found = false;
	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (!dot_found) {
			if (*i == '.')
				dot_found = true;
		}
		else {
			if (*i == '\\' || *i == '/')
				break;
			buffer.push_back(*i);
		}
	}
	std::reverse(buffer.begin(), buffer.end());
	return buffer;
}

std::string FileSystem::GetFileFromPath(const char * file) const
{
	std::string f(file);
	std::string buffer;

	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (*i == '\\' || *i == '/')
			break;
		buffer.push_back(*i);
	}
	std::reverse(buffer.begin(), buffer.end());

	return buffer;
}

std::string FileSystem::GetFileDirectory(const char * file_path) const
{
	std::string f(file_path);

	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (*i == '\\' || *i == '/') {
			f.erase(i.base(), f.end());
			break;
		}
	}

	return f;
}

bool FileSystem::IsFileSupported(const char * path) const
{
	FileSystem::Format form = GetFormat(path);
	return form == FileSystem::Format::DDS || form == FileSystem::Format::FBX || form == FileSystem::Format::JPG || form == FileSystem::Format::PNG;
}

bool FileSystem::CreateDir(const char * path)
{
	if (std::experimental::filesystem::create_directories(path)) {
		LOG("Directory %s created successfully", path);
		return true;
	}
	else {
		LOG("Faile to create directory %s");
		return false;
	}
}

bool FileSystem::RemoveFile(const char * path)
{
	if (std::experimental::filesystem::remove(path) == true) {
		LOG("Removed %s successfully", path);
		return true;
	}
	else {
		LOG("Failed to delete file %s", path);
		return false;
	}
}

bool FileSystem::RemoveDirAndContent(const char * path)
{
	if (std::experimental::filesystem::remove_all(path) == 0) {
		LOG("Removed %s successfully", path);
		return true;
	}
	else {
		LOG("Failed to delete dir %s", path);
		return false;
	}
	return false;
}

bool FileSystem::IsInDirectory(const char * directory, const char * file) const
{
	struct stat buffer;
	return (stat(std::string(std::string(directory) + file).c_str(), &buffer) == 0);
}

bool FileSystem::IsInSubDirectory(const char * directory, const char * file, std::string * const real_path) const
{
	std::string path = directory;
	
	return RecursiveDirectory(path.c_str(), file, real_path);
}

bool FileSystem::RecursiveDirectory(const char* path, const char * file, std::string * const real_path) const
{
	bool ret = false;
	for (const auto & entry : std::experimental::filesystem::directory_iterator(path))
		if (std::experimental::filesystem::is_directory(entry)) {
			if (RecursiveDirectory(entry.path().u8string().c_str(), file, real_path))
				return true;
		}
		else {
			if (entry.path().filename().u8string().compare(file) == 0) {
				if (real_path != nullptr)
					(*real_path).assign(entry.path().u8string());
				return true;
			}
		}

	return ret;
}

bool FileSystem::Exists(const char * path) const
{
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool FileSystem::IsMetaVaild(const char * path)
{
	char* ret = GetData(path);

	if (ret == nullptr) {
		return false;
	}
	else {
		uint64_t uid = 0u;
		memcpy(&uid, ret, sizeof(uint64_t));
		delete[] ret;
		if (uid == 0u) {
			return false;
		}
		else {
			if (Exists((MATERIAL_L_FOLDER + std::to_string(uid) + ".dds").c_str()) || Exists((MODEL_L_FOLDER + std::to_string(uid) + ".whispModel").c_str()))
				return true;
			return false;
		}
	}
}

bool FileSystem::HasMeta(const char * file)
{
	return Exists((std::string(file) + ".meta").c_str());
}

uint64_t FileSystem::GenerateMetaFile(const char * meta_path)
{
	return GenerateMetaFile(meta_path, App->random->RandomGUID());
}

uint64_t FileSystem::GenerateMetaFile(const char * file_path, const uint64 & force_uid, const void * addition_data, const uint& size_addition_data)
{
	char* meta_data = new char[sizeof(uint64_t) + size_addition_data];
	memcpy(meta_data, &force_uid, sizeof(uint64_t));
	if (size_addition_data > 0u) {
		char* cursor = meta_data + sizeof(uint64_t);
		memcpy(cursor, addition_data, size_addition_data);
	}
	App->dummy_file_system->SaveData(meta_data, sizeof(uint64_t) + size_addition_data, (std::string(file_path) + ".meta").c_str());
	delete[] meta_data;

	return force_uid;
}

uint64_t FileSystem::GetUIDMetaFrom(const char * mata_path) const
{
	char* f_uid = App->dummy_file_system->GetData(mata_path);
	if (f_uid == nullptr) {
		LOG("Failed to open meta file");
		return 0u;
	}
	uint64_t uid = 0u;
	memcpy(&uid, f_uid, sizeof(uint64_t));

	delete[] f_uid;
	return uid;
}

bool FileSystem::SaveData(const void * data, const uint &size, const char * path)
{
	std::ofstream to_save(path, std::ios::binary);
	to_save.write((char*)data, size);
	to_save.close();

	return true;
}

char * FileSystem::GetData(const char * path)
{
	std::ifstream file(path, std::ios::binary);

	if (!file) {
		LOG("Failed to open %s", path);
		file.close();
		return nullptr;
	}

	file.seekg(0, std::ios::end);
	size_t len = file.tellg();
	char *data = new char[len];
	file.seekg(0, std::ios::beg);
	file.read(data, len);
	file.close();

	return data;
}
