#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include <list>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "DockPanel.h"

class ConsoleDockPanel;
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
	void DrawPanels();

	void AddaptToFlySection(FlyEngineSection flyEngineSection); 

	DockPanel* GetDockPanel(DockPanelType panelType);

	ConsoleDockPanel* consoleDockPanel;
	ImFont* engineFont;
	ImFont* buttonFont;

private: 

	// Fonts & Style 
	void SetStyle(); 

	std::list<DockPanel*> dockPanels;
	ImGuiID mainDock_ID = 0; 
};

#endif
