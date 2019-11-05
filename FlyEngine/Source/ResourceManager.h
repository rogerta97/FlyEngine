#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <list>

using namespace std; 

class Texture; 
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
	std::list<*> 
};

#endif // !_MODULE_GAME_VIEWPORT_H_


