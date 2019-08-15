#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"

class Application
{
public:

	ModuleInput* moduleInput; 
	ModuleWindow* moduleWindow;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;

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