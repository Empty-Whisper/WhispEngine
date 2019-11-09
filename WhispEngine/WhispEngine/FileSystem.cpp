#include "FileSystem.h"
#include <fstream>
#include <iomanip>
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
	if (i.fail())
		return nullptr;

	i >> file;

	return file;
}

void FileSystem::SaveFile(const char * path, const nlohmann::json & to_save)
{
	std::ofstream o(path);
	o << std::setw(4) << to_save << std::endl;
	o.close();
}

FileSystem::Format FileSystem::GetFormat(const char * file)
{
	std::string f(file);
	std::string buffer;

	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (*i != '.')
			buffer.push_back(std::tolower(*i));
		else
			break;
	}
	std::reverse(buffer.begin(), buffer.end());

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
	
	LOG("Cannot identify format, format is: %s", buffer.data());

	return FileSystem::Format::NONE;
}

std::string FileSystem::GetFileNameFromPath(const char * file)
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

std::string FileSystem::GetFileFromPath(const char * file)
{
	std::string f(file);
	std::string buffer;

	for (auto i = f.rbegin(); i != f.rend(); i++) {
		if (*i == '\\' || *i == '/')
			break;
		buffer.push_back(*i);
	}
	std::reverse(buffer.begin(), buffer.end());

	return buffer.data();
}

bool FileSystem::IsInDirectory(const char * directory, const char * file)
{
	struct stat buffer;
	return (stat(std::string(std::string(directory) + file).c_str(), &buffer) == 0);
}

bool FileSystem::SaveData(const char * data, const char * path, const uint &size)
{
	std::ofstream to_save(path);
	to_save.write(data, size);
	to_save.close();

	return true;
}

char * FileSystem::GetData(const char * path)
{
	std::ifstream file(path);

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
