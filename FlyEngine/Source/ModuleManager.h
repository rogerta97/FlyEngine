#ifndef _MODULEMANAGER_H_
#define _MODULEMANAGER_H_

#include "Module.h"
#include "Globals.h"

class ModuleManager : public Module
{
public:
	ModuleManager(bool start_enabled = true);
	~ModuleManager();

	bool Init();

};

#endif