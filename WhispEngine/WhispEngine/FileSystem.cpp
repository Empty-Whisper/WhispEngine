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
			buffer.push_back(*i);
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
			if (*i == '\\')
				break;
			buffer.push_back(*i);
		}
	}
	std::reverse(buffer.begin(), buffer.end());
	return buffer.data();
}
