#include "FileSystem.h"

FileSystem* FileSystem::instance = 0; 

FileSystem::FileSystem() {
	solutionDirectory = SOLUTION_DIR; 
}

FileSystem* FileSystem::getInstance()
{
	if (instance == 0)
		instance = new FileSystem();
	
	return instance;
}

FileSystem::~FileSystem() {

}

string FileSystem::GetLastPathItem(string path, bool keepTermination = true)
{
	string resultString(path);

	int pos = resultString.find_last_of('\\');
	int to_copy = resultString.length() - pos;
	resultString = resultString.substr(pos + 1, to_copy);

	if (keepTermination == false)
		DeleteFileExtension(resultString);

	return resultString;
}

void FileSystem::DeleteFileExtension(string& path)
{
	int pos = path.find_first_of(".");
	int to_del = path.size() - pos;
	path = path.substr(0, path.size() - to_del);
}

void FileSystem::Delete()
{
	delete instance; 
}
