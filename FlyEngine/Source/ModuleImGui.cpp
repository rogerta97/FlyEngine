#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"

#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "ConsoleDockPanel.h"
#include "RoomsGraphDockPanel.h"
#include "WorldPropertiesDockPanel.h"
#include "GraphPropertiesDockPanel.h"
#include "FileSystem.h"

#include "GameViewportDockPanel.h"
#include "RoomObjectsDockPanel.h"
#include "ObjectPropertiesDockPanel.h"

#include <string>
#include <Windows.h>
#include "mmgr.h"

ModuleImGui::ModuleImGui(bool start_enabled)
{
	moduleType = MODULE_IMGUI;
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	

	return true;
}

bool ModuleImGui::Start()
{
	name = "ImGui";

	ImGui_ImplSDL2_InitForOpenGL(App->moduleWindow->mainWindow, App->moduleRender->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	string fontsDirectory = FileSystem::getInstance()->solutionDirectory + "EngineResources/Fonts/Exo-Regular.otf";
	engineFont = io.Fonts->AddFontFromFileTTF(fontsDirectory.c_str(), 16);
	buttonFont = io.Fonts->AddFontFromFileTTF(fontsDirectory.c_str(), 24);

	CreatePanels();
	SetStyle();
	return true; 
}

void ModuleImGui::CreatePanels()
{
	ConsoleDockPanel* consolePanel = new ConsoleDockPanel(true);
	RoomsGraphDockPanel* roomsGraphPanel = new RoomsGraphDockPanel(true);
	GraphPropertiesDockPanel* graphPropertiesPanel = new GraphPropertiesDockPanel(true);
	WorldPropertiesDockPanel* worldPropertiesDockPanel = new WorldPropertiesDockPanel(true);

	GameViewportDockPanel* gameViewportDockPanel = new GameViewportDockPanel(false);
	RoomObjectsDockPanel* roomObjectsDockPanel = new RoomObjectsDockPanel(false);
	ObjectPropertiesDockPanel* objectPropertiesDockPanel = new ObjectPropertiesDockPanel(false);

	dockPanels.push_back(consolePanel); 
	dockPanels.push_back(roomsGraphPanel);
	dockPanels.push_back(graphPropertiesPanel);
	dockPanels.push_back(worldPropertiesDockPanel); 

	dockPanels.push_back(gameViewportDockPanel);
	dockPanels.push_back(roomObjectsDockPanel);
	dockPanels.push_back(objectPropertiesDockPanel);

	consoleDockPanel = consolePanel; 
	graphPropertiesDockPanel = graphPropertiesPanel; 
}

void ModuleImGui::DeletePanels()
{
	for (auto it = dockPanels.rbegin(); it != dockPanels.rend(); it++) {
		(*it)->CleanUp(); 
		delete((*it));
		(*it) = nullptr;
	}

	dockPanels.clear(); 
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->moduleWindow->mainWindow);
	ImGui::NewFrame();

	return update_status::UPDATE_CONTINUE;
}

void ModuleImGui::DrawDockSpace() 
{	
	bool open = true;

	// Set Main Window Settings
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));

	// Create Main Window 
	ImGui::Begin("MainDock", &open, window_flags);
	ImGui::PopStyleVar(2);

	mainDock_ID = ImGui::GetID("MainDock");

	// Edit Menu Bar 
	DrawMainMenuBar();

	// Create the dockspace 
	ImGui::SetNextWindowDockID(mainDock_ID);
	ImGui::DockSpace(mainDock_ID);

	// Draw needed panels 
	DrawPanels(); 

	ImGui::End(); 
}

void ModuleImGui::DrawMainMenuBar()
{
	ImGui::BeginMainMenuBar();

	static bool demoOpened = false;

	if (ImGui::MenuItem("File")) {

	}

	if (ImGui::MenuItem("Edit")) {

	}

	if (ImGui::BeginMenu("View")) {

		for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {
			if (ImGui::MenuItem((*it)->GetName().c_str(), "", (*it)->IsVisible())){
				(*it)->ToggleVisibility();
			}
		}

		// TODO: Rude Way to Show Demo Window
		if (ImGui::MenuItem("ImGui Demo")){
			demoOpened = !demoOpened; 
		}
		
		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

	if (demoOpened)
		ImGui::ShowDemoWindow(&demoOpened);
}

void ModuleImGui::DrawPanels()
{
	for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {
		(*it)->Draw();
	}
}

void ModuleImGui::AddaptToFlySection(FlyEngineSection flyEngineSection)
{
	for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {

		if ((*it)->GetFlySection() == flyEngineSection || (*it)->GetFlySection() == FLY_SECTION_BOTH)
			(*it)->SetVisible(true);
		else
			(*it)->SetVisible(false); 
	}
}

DockPanel* ModuleImGui::GetDockPanel(DockPanelType panelType)
{
	for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {
		
		if ((*it)->GetPanelType() == panelType)
			return (*it);
	}
}

update_status ModuleImGui::Update(float dt)
{
	DrawDockSpace(); 

	
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	DeletePanels(); 
	ImGui_ImplSDL2_Shutdown(); 
	return true;
}

void ModuleImGui::SetStyle() {

	ImGui::StyleColorsDark();

	ImGuiStyle * style = &ImGui::GetStyle();
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(7.0f, 7.0f);
	style->WindowPadding = ImVec2(7.0f, 7.0f);

 //
	//style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	//style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	//style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	//style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	//style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	//style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	//style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	//style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	//style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	////style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	////style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	////style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	//style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	//style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	////style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	////style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	////style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	//style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	//style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	//style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	//style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

//void ModuleImGui::CreateDock()
//{
//	ImGuiID dockspace_id = ImGui::GetID("MainDock");
//	ImGuiViewport* viewport = ImGui::GetMainViewport();
//
//	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
//	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
//	ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);  // Add empty node
//
//	ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
//	ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, nullptr, &dock_main_id);
//	ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
//	ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, nullptr, &dock_main_id);
//
//	//ImGui::DockBuilderDockWindow(SystemLog::ID.c_str(), dock_id_bottom);
//	//ImGui::DockBuilderDockWindow(FormManager::PROPERTY_EDITOR_ID.c_str(), dock_id_right);
//	//ImGui::DockBuilderDockWindow(FormManager::PROJECT_EXPLORER_ID.c_str(), dock_id_left);
//	//ImGui::DockBuilderDockWindow("Extra", dock_id_prop);
//
//	//ImGui::DockBuilderDockWindow(fmt::format("dummy###{0}", FilelistForm::ID).c_str(), dock_id_left);
//	//ImGui::DockBuilderDockWindow(fmt::format("dummy###{0}", AudioPlayerForm::ID).c_str(), dock_main_id);
//	//ImGui::DockBuilderDockWindow(fmt::format("dummy###{0}", PlaylistForm::ID).c_str(), dock_id_right);
//	//ImGui::DockBuilderDockWindow(SystemLog::ID.c_str(), dock_id_bottom);
//
//	ImGui::DockBuilderDockWindow("hjghj", dockspace_id); 
//	ImGui::DockBuilderFinish(dockspace_id);
//}

