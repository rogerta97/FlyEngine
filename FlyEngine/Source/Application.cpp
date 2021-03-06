#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleImGui.h"
#include "ModuleRender.h"
#include "ModuleWorldManager.h"
#include "ModuleManager.h"
#include "ModuleAudioManager.h"
#include "mmgr.h"

Application::Application()
{
	gameMode = ENGINE_MODE; 

	moduleInput = new ModuleInput(this);
	moduleWindow = new ModuleWindow(this);
	moduleImGui = new ModuleImGui(this);
	moduleWorldManager = new ModuleWorldManager(this);
	moduleRender = new ModuleRender(this);
	moduleManager = new ModuleManager(this); 
	moduleAudioManager = new ModuleAudioManager(this); 

	AddModule(moduleInput);
	AddModule(moduleWindow);

	AddModule(moduleWorldManager);
	AddModule(moduleRender);
	AddModule(moduleImGui);
	AddModule(moduleAudioManager);
	AddModule(moduleManager); 

	isEngineInPlayMode = false; 
}

Application::~Application()
{
	std::list<Module*>::iterator module_iterator = list_modules.begin();

	while (module_iterator != list_modules.end())
	{
		delete* module_iterator;
		module_iterator++;
	}
}

float Application::GetDeltaTime()
{
	return dt;
}

bool Application::Init()
{
	bool ret = true;

	// Init() in all modules
	std::list<Module*>::iterator module_iterator = list_modules.begin();
	while (module_iterator != list_modules.end() && ret == true)
	{
		ret = (*module_iterator)->Init();
		module_iterator++;
	}

	// Start() in all modules
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
	
	SaveAndLoad::getInstance()->SaveInitFile();

	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->CleanUp();
		}
	}

	return ret;
}

void Application::Exit()
{
	App->moduleInput->quit = true;
}

void Application::BroadCastEvent(FlyEngineEvent eventType)
{
	for (auto& it : list_modules) 
	{
		(it)->ReceiveEvent(eventType); 
	}
}

void Application::SetGameMode(GameMode newGameMode)
{
	gameMode = newGameMode; 
}

GameMode Application::GetGameMode()
{
	return gameMode;
}

void Application::SetIsReleaseVersion(bool newGameMode)
{
	isReleaseVersion = newGameMode;
}

bool Application::IsReleaseVersion()
{
	return isReleaseVersion;
}



void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}