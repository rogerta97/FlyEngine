#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleRender.h"

#include <list>

enum EngineWindows {

	ROOM_OVERVIEW, 
	ROOM_EDIT,
};

class Application
{
public:
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