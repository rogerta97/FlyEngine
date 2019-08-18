#pragma once

#include "Globals.h"
#include "Module.h"
#include "Timer.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
//#include "ModuleImGui.h"

#include <list>

enum EngineWindow {
	EW_ROOM_VISUALIZATION, 
	EW_ROOM_EDITING, 
};

class Application
{
public:

	ModuleInput* moduleInput; 
	ModuleWindow* moduleWindow;
	ModuleRender* moduleRender; 
	//ModuleImGui* moduleImGui; 

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

	EngineWindow currentEngineWindow; 

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