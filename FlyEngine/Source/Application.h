#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <list>

enum EngineWindows {

	ROOM_OVERVIEW, 
	ROOM_EDIT,
};

// Forward Declarations 
class ModuleWindow;
class ModuleInput;
class ModuleImGui;
class ModuleRender;
class ModuleWorldManager;

class Application
{
public:

	ModuleWorldManager* moduleWorldManager; 
	ModuleWindow* moduleWindow;
	ModuleInput* moduleInput;
	ModuleImGui* moduleImGui;

	ModuleRender* moduleRender;

private:

	Timer	ms_timer;
	float	dt;
	EngineWindows engineWindow; 
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};