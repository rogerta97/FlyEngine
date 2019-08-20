#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include "Module.h"
#include "Globals.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

private: 

	void DrawTopBar(); 
	void SetStyle(); 
};

#endif
