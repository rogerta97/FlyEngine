#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <list>

using namespace std; 

class Resource; 
struct ResourceManager
{
private:
	static ResourceManager* instance;
	ResourceManager();

public:
	static ResourceManager* getInstance();
	~ResourceManager();

	// Utility --------------------------------
	void Delete();

private: 
	std::list<Resource*> resourceList; 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


