#include "MyFileSystem.h"
#include "SDL/include/SDL_filesystem.h"

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


string MyFileSystem::DeleteLastPathItem(string path)
{
	string resultString(path);

	int pos = resultString.find_last_of('\\');

	if (pos == path.size() - 1)
	{
		string nextStr = resultString.substr(0, pos); 
		DeleteLastPathItem(nextStr.c_str());
	}

	resultString = resultString.substr(0, pos + 1);

	return resultString;
}

void MyFileSystem::DeleteFileExtension(string& path)
{
	int pos = path.find_first_of(".");
	int to_del = path.size() - pos;
	path = path.substr(0, path.size() - to_del);
}

string MyFileSystem::GetIconsDirectory()
{
	if (iconsDirectory == "")
	{
		iconsDirectory = GetSolutionDirectory(); 
		iconsDirectory += "FlyEngine\\Source\\Game\\Resources\\EngineIcons\\"; 
	}
		
	return iconsDirectory;
}

string MyFileSystem::GetSolutionDirectory()
{
	if (solutionDirectory == "")
	{
		solutionDirectory = SDL_GetBasePath();
		solutionDirectory = DeleteLastPathItem(solutionDirectory); 
	}

	return solutionDirectory;
}

void MyFileSystem::Delete()
{
	delete instance; 
}
