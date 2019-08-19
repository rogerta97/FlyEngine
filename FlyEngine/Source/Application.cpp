#include "Application.h"

Application::Application()
{
	moduleInput = new ModuleInput(this); 
	moduleWindow = new ModuleWindow(this);
	moduleRender = new ModuleRender(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(moduleInput);
	AddModule(moduleWindow);
	AddModule(moduleRender);
	
	// Scenes

	// Renderer last!
}

Application::~Application()
{
	std::list<Module*>::iterator module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end())
	{
		delete *module_iterator; 
		module_iterator++; 
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	std::list<Module*>::iterator module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end() && ret == true)
	{
		ret = (*module_iterator)->Init();
		module_iterator++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	module_iterator = list_modules.begin();
	while (module_iterator != list_modules.end() && ret == true)
	{
		ret = (*module_iterator)->Start();
		module_iterator++;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{	
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->CleanUp();
		}
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}