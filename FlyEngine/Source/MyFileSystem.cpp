#include <algorithm>

#include "MyFileSystem.h"
#include "SDL/include/SDL_filesystem.h"
#include "imgui.h"
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


bool MyFileSystem::IsFolder(string& directory)
{
	vector<string> files;
	instance->GetFilesInDirectory(directory.c_str(), files, false);

	for (auto it = files.begin(); it != files.end(); it++)
	{
		if ((*it) != "." && (*it) != "..")
		{
			return true;
		}
	}

	return false;
}


string MyFileSystem::GetLastPathItem(string path, bool keepTermination = true)
{
	string resultString(path);

	int pos = resultString.find_last_of('\\');
	int to_copy = resultString.length() - pos;
	resultString = resultString.substr(pos + 1, to_copy);

	if (keepTermination == false)
		resultString = DeleteFileExtension(resultString);

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

std::string MyFileSystem::DeleteFileExtension(string path)
{
	int pos = path.find_first_of(".");
	int to_del = path.size() - pos;
	path = path.substr(0, path.size() - to_del);
	return path; 
}

FileExtension MyFileSystem::GetFileExtension(string path)
{
	int pos = path.find_first_of(".");
	int to_del = path.size() - pos;
	path = path.substr(pos, to_del);

	if (path == ".png" || path == ".PNG")
		return FILE_PNG;

	if (path == ".jpg" || path == ".JPG")
		return FILE_JPG;

	if (path == ".mp3" || path == ".MP3")
		return FILE_MP3;

	if (path == ".wav" || path == ".WAV")
		return FILE_WAV;

	if (path == ".ttf" || path == ".TTF")
		return FILE_TTF;

	if (path == ".otf" || path == ".OTF")
		return FILE_OTF;
}

string MyFileSystem::GetIconsDirectory()
{
	if (instance->iconsDirectory == "")
	{
		instance->iconsDirectory = GetSolutionDirectory();
		instance->iconsDirectory += "Source\\Game\\Resources\\EngineResources\\EngineIcons\\";
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

string MyFileSystem::GetResourcesDirectory()
{
	std::string resourcesDir = GetSolutionDirectory(); 
	resourcesDir += "Source\\Game\\Resources";
	return resourcesDir;
}

string MyFileSystem::GetSavedDataDirectory()
{
	return instance->GetSolutionDirectory() + "Source\\Game\\Resources\\EngineResources\\EngineSavedData\\";
}

string MyFileSystem::GetDialogueNodesDirectory()
{
	return instance->GetSolutionDirectory() + "Source\\Game\\Resources\\EngineResources\\EngineSavedData\\NodeGraphData\\Dialogues\\";
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

void MyFileSystem::GetFilesInDirectory(const char* directory, std::vector<string>& list, bool include_path)
{
	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (std::string(data.cFileName) != std::string(".") && std::string(data.cFileName) != std::string(".."))
			{
				if (include_path)
				{
					string new_str = directory + string("\\") + string(data.cFileName);
					list.push_back(new_str);
				}
				else
					list.push_back(data.cFileName);
			}


		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return;
}

std::vector<string> MyFileSystem::GetFoldersInDirectory(const char* directory)
{
	vector<string> retList; 
	std::string path(directory);
	path.append("\\*");

	WIN32_FIND_DATA data;
	HANDLE hFind;

	if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (std::string(data.cFileName) != std::string(".") && std::string(data.cFileName) != std::string(".."))
			{	
				string new_str = directory + string("\\") + string(data.cFileName);
				
				if (instance->IsFolder(new_str))
				{
					retList.push_back(new_str);
				}
			}

		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return retList;
}

void MyFileSystem::Init()
{
	CreateDirectory(string(GetSavedDataDirectory() + "RoomsData").c_str(), NULL);
	CreateDirectory(string(GetSavedDataDirectory() + "BlackboardsData").c_str(), NULL);
	CreateDirectory(string(GetSavedDataDirectory() + "NodeGraphData").c_str(), NULL);
	CreateDirectory(string(GetSavedDataDirectory() + "NodeGraphData\\Dialogues").c_str(), NULL);
}

void MyFileSystem::Delete()
{
	delete instance; 
}
