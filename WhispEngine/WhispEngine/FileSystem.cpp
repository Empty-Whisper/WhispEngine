#include "FileSystem.h"
#include <fstream>
#include <iomanip>


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
