#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <list>
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
	bool AddResource(Resource* newResource); 

	bool ExistResource(Resource* checkResource); 
	bool ExistResource(UID resourceUID); 

	void Delete();

private: 
	std::list<Resource*> resourceList; 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


