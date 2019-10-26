#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include <list>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "DockPanel.h"

class ConsoleDockPanel;
class GraphPropertiesDockPanel; 

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	// Module 
	bool Init(); 
	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt); 
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
	GraphPropertiesDockPanel* graphPropertiesDockPanel; 

	ImFont* defaultFont;
	ImFont* headerFont;

private: 

	// Fonts & Style 
	void SetStyle();
	void DarkTheme();
	void CherryTheme();

	std::list<DockPanel*> dockPanels;
	ImGuiID mainDock_ID = 0; 
};

#endif
