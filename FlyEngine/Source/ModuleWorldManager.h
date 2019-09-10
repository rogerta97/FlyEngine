#ifndef  _MODULEENGINEMANAGER_H_
#define _MODULEENGINEMANAGER_H_

#include "Module.h"
#include "Globals.h"

#include <list>

using namespace std; 

class Room; 
class ModuleWorldManager : public Module
{
public:

	ModuleWorldManager(bool start_enabled = true);
	~ModuleWorldManager();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:

	list<Room*> roomsInWorldList; 
};


#endif // ! _MODULEENGINEMANAGER_H_

