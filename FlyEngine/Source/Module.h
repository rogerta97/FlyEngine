#ifndef _MODULE_H_
#define _MODULE_H_

#include "Globals.h"

class Application;
struct PhysBody3D;

enum ModuleType
{
	MODULE_ENGINE_MANAGER, 
	MODULE_INPUT, 
	MODULE_RENDER, 
	MODULE_WINDOW, 
	MODULE_IMGUI, 
	MODULE_null
};

class Module
{
private :
	bool enabled;

protected: 
	ModuleType moduleType = MODULE_null; 

public:
	const char* name = "";

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}
};

#endif

extern Application* App;