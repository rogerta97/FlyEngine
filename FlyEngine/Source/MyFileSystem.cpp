#include <algorithm>

#include "MyFileSystem.h"
#include "SDL/include/SDL_filesystem.h"
#include "mmgr.h"

MyFileSystem* MyFileSystem::instance = 0; 

MyFileSystem::MyFileSystem() 
{

}

MyFileSystem* MyFileSystem::getInstance()
{
	if (instance == 0)
		instance = new MyFileSystem();
	
	return instance;
}

MyFileSystem::~MyFileSystem() {

}

string MyFileSystem::GetLastPathItem(string path, bool keepTermination = true)
{
	string resultString(path);

	int pos = resultString.find_last_of('\\');
	int to_copy = resultString.length() - pos;
	resultString = resultString.substr(pos + 1, to_copy);

	if (keepTermination == false)
		DeleteFileExtension(resultString);

	return resultString;
}


void MyFileSystem::DeleteLastPathItem(string& path)
{
	int pos = path.find_last_of('\\');

	if (pos == path.size() - 1)
	{
		path = path.substr(0, pos);
		pos = path.find_last_of('\\');
	}

	path = path.substr(0, pos + 1);
}

void MyFileSystem::DeleteFrontItem(string& path)
{
	const char bars[] = 
	{
		'\\', 
		'/', 
	}; 

	int firstBarPos = path.find_first_of('\\');
	int pathSize = path.size() - 1 - firstBarPos; 
	path = path.substr(firstBarPos + 1, pathSize); 
}

void MyFileSystem::DeleteFileExtension(string& path)
{
	int pos = path.find_first_of(".");
	int to_del = path.size() - pos;
	path = path.substr(0, path.size() - to_del);
}

string MyFileSystem::GetIconsDirectory()
{
	if (instance->iconsDirectory == "")
	{
		instance->iconsDirectory = GetSolutionDirectory();
		instance->iconsDirectory += "Source\\Game\\Resources\\EngineIcons\\";
	}
		
	return instance->iconsDirectory;
}

string MyFileSystem::GetSolutionDirectory()
{
	if (instance->solutionDirectory == "")
	{
		instance->solutionDirectory = SDL_GetBasePath();
		DeleteLastPathItem(instance->solutionDirectory);
	}

	return instance->solutionDirectory;
}

string MyFileSystem::GetGameDirectory()
{
	if (instance->gameDirectory == "")
	{
		instance->gameDirectory = instance->GetSolutionDirectory();
		instance->gameDirectory += "Source\\Game"; 
	}

	return instance->gameDirectory;
}

void MyFileSystem::GetRelativeDirectory(string& directory)
{
	instance->DeleteFrontItem(directory);

	if (instance->GetBarsCount(directory) == 2)
		return; 
	else
		instance->GetRelativeDirectory(directory);
}

int MyFileSystem::GetBarsCount(string countStr)
{
	return (int)std::count(countStr.begin(), countStr.end(), '\\');
}

void MyFileSystem::Delete()
{
	delete instance; 
}
