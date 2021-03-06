#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "Globals.h"
#include "Timer.h"
#include "Module.h"

#include <list>

enum FlyEngineSection
{
	FLY_SECTION_WORLD_EDITOR,
	FLY_SECTION_ROOM_EDIT,
	FLY_SECTION_BOTH,
	FLY_SECTION_null
};

enum GameMode
{
	ENGINE_MODE,
	PLAY_MODE,
};

// Forward Declarations 
class ModuleWindow;
class ModuleInput;
class ModuleImGui;
class ModuleRender;
class ModuleWorldManager;
class ModuleManager; 
class ModuleAudioManager; 

class Application
{
public:

	ModuleWorldManager* moduleWorldManager; 
	ModuleWindow* moduleWindow;
	ModuleInput* moduleInput;
	ModuleRender* moduleRender;
	ModuleImGui* moduleImGui;
	ModuleManager* moduleManager; 
	ModuleAudioManager* moduleAudioManager; 

private:

	bool isReleaseVersion;
	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	GameMode gameMode; 

public:

	Application();
	~Application();

	float GetDeltaTime(); 

	bool Init();
	update_status Update();
	bool CleanUp();
	
	void Exit(); 

	void BroadCastEvent(FlyEngineEvent eventType); 

	void SetGameMode(GameMode newGameMode); 
	GameMode GetGameMode();

	void SetIsReleaseVersion(bool newGameMode);
	bool IsReleaseVersion();

	FlyEngineSection flySection = FlyEngineSection::FLY_SECTION_null; 
	bool isEngineInPlayMode = false; 

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

#endif