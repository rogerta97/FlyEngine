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
class ModuleRoomManager;
class ModuleManager; 
class ModuleAudioManager; 

class Application
{
public:

	ModuleRoomManager* moduleRoomManager; 
	ModuleWindow* moduleWindow;
	ModuleInput* moduleInput;
	ModuleRender* moduleRender;
	ModuleImGui* moduleImGui;
	ModuleManager* moduleManager; 
	ModuleAudioManager* moduleAudioManager; 

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

	void BroadCastEvent(FlyEngineEvent eventType); 

	FlyEngineSection flySection = FlyEngineSection::FLY_SECTION_null; 
	bool isEngineInPlayMode = false; 

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif