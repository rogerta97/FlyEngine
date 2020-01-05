#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <list>
#include <string>
#include "Resource.h"
#include "Globals.h"

using namespace std; 

struct ResourceManager
{
private:
	static ResourceManager* instance;
	ResourceManager();

public:
	static ResourceManager* getInstance();
	~ResourceManager();

	// Utility --------------------------------
	static bool AddResource(Resource* newResource, std::string name);

	static Resource* GetResource(std::string resourceName);
	static Resource* GetResource(UID resourceUID);
	static Resource* GetResourceByPath(std::string resourcePath);

	static bool ExistResource(Resource* checkResource);
	static bool ExistResourceUID(UID resourceUID);
	static bool ExistResourcePath(std::string resourcePath);

	// Default Loads 
	static void LoadResource(string newResourcePath); 
	static void LoadAllGameResources(); 
	static void LoadAllFilesFromFolder(string path); 

	static void LoadEngineIconsResources(); 

	static void CleanUp();

private: 
	std::list<Resource*> resourceList; 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


