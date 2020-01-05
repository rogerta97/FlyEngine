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
	static ResourceType GetResourceTypeFromExtension(FileExtension ext); 

	static Resource* GetResource(std::string resourceName, ResourceType type = ResourceType::RESOURCE_null);
	static Resource* GetResource(UID resourceUID);
	static Resource* GetResourceByPath(std::string resourcePath);

	static bool ExistResource(Resource* checkResource);
	static bool ExistResourceUID(UID resourceUID);
	static bool ExistResourcePath(std::string resourcePath);

	// User Resources Loads 
	static void LoadResource(string newResourcePath, ResourceType forceType = ResourceType::RESOURCE_null);
	static void LoadAllFilesFromFolder(string path, ResourceType forceType = ResourceType::RESOURCE_null);
	
	static void LoadAllGameResources(); 

	// Engine Resources Loads
	static void LoadEngineIconsResources(); 

	static void CleanUp();

private: 
	std::list<Resource*> resourceList; 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


