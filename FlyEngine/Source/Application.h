#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <list>

enum FlyEngineSection
{
	FLY_SECTION_ROOM_GRAPH,
	FLY_SECTION_ROOM_EDIT,
	FLY_SECTION_BOTH,
	FLY_SECTION_null
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
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	FlyEngineSection flySection = FlyEngineSection::FLY_SECTION_null; 

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif