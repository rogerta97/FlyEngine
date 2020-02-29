#ifndef _MODULEIMGUI_H_
#define _MODULEIMGUI_H_

#include <list>
#include <vector>

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "DockPanel.h"

#define PUSH_FONT(font) ImGui::PushFont(font)
#define POP_FONT ImGui::PopFont();
#define IMGUI_SPACED_SEPARATOR ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

class ConsoleDockPanel;
class GraphPropertiesDockPanel; 
class GameViewportDockPanel;
class ObjectCreatorDockPanel; 

class ModuleImGui : public Module
{
public:
	ModuleImGui(bool start_enabled = true);
	~ModuleImGui();

	// Module ---------------
	bool Init(); 
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt); 
	void ReceiveEvent(FlyEngineEvent eventType); 
	bool CleanUp();

	// Docking --------------
	void DrawDockSpace();
	void DrawMainMenuBar();

	// Panels ---------------
	void CreatePanels(); 
	void DeletePanels(); 
	void DrawPanels();

	void AddaptToFlySection(FlyEngineSection flyEngineSection); 

	DockPanel* GetDockPanel(DockPanelType panelType);

	ConsoleDockPanel* consoleDockPanel;
	GraphPropertiesDockPanel* graphPropertiesDockPanel;
	GameViewportDockPanel* gameViewportDockPanel;
	ObjectCreatorDockPanel* objectCreatorDockPanel; 

	// Fonts ---------------
	ImFont* rudaRegularMid;
	ImFont* rudaBoldMid;
	ImFont* rudaBlackMid;

	ImFont* rudaRegularSmall;
	ImFont* rudaBoldSmall;
	ImFont* rudaBlackSmall;

	ImFont* rudaRegularTiny;
	ImFont* rudaBoldTiny;
	ImFont* rudaBlackTiny;

	ImFont* rudaRegularBig;
	ImFont* rudaBoldBig;
	ImFont* rudaBlackBig;

	ImFont* rudaRegularHuge;
	ImFont* rudaBoldHuge;
	ImFont* rudaBlackHuge;

	ImFont* rudaRegularGiant;
	ImFont* rudaBoldGiant;
	ImFont* rudaBlackGiant;

private: 

	// Fonts & Style 
	void SetStyle();
	void DarkTheme();
	void CherryTheme();

	std::list<DockPanel*> dockPanels;
	ImGuiID mainDock_ID = 0; 
};

#endif
