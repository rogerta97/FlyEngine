#pragma once

#include "Globals.h"
#include "Module.h"
#include "Timer.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"

#include <list>

class Application
{
public:

	ModuleInput* moduleInput; 
	ModuleWindow* moduleWindow;
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

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};