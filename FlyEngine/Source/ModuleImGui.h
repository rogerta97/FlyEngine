#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include <list>

#include "Module.h"
#include "Globals.h"
#include "imgui.h"

class DockPanel; 
class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	// Module 
	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	// Docking
	void DrawDockSpace();
	void DrawMainMenuBar();

	// Panels 
	void CreatePanels(); 
	void DeletePanels(); 

	void AddPanelToRenderList(DockPanel* newPanel);
	void DrawPanels(); 

private: 

	// Utility 
	void SetStyle(); 

	std::list<DockPanel*> activeDockPanels; 
	ImGuiID mainDock_ID = 0; 
};

#endif
