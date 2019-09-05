#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"

#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "SDL_opengl.h"

#include "ConsoleDockPanel.h"
#include "RoomGraphDockPanel.h"
#include "ProjectInfoDockPanel.h"

#include <string>
#include <Windows.h>

ModuleImGui::ModuleImGui(bool start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Start()
{
	name = "ImGui";

	ImGui_ImplSDL2_InitForOpenGL(App->moduleWindow->mainWindow, App->moduleRender->context);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

	CreatePanels(); 

	return true;
}

void ModuleImGui::CreatePanels()
{
	ConsoleDockPanel* consolePanel = new ConsoleDockPanel();
	RoomGraphDockPanel* roomGraphPanel = new RoomGraphDockPanel();
	ProjectInfoDockPanel* projectInfoPanel = new ProjectInfoDockPanel();

	AddPanelToRenderList(consolePanel); 
	AddPanelToRenderList(roomGraphPanel);
	AddPanelToRenderList(projectInfoPanel);
}

void ModuleImGui::DeletePanels()
{
	for (auto it = activeDockPanels.rbegin(); it != activeDockPanels.rend(); it++) {
		delete((*it));
		(*it) = nullptr; 
	}

	activeDockPanels.clear(); 
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

	if (ImGui::MenuItem("File")) {

	}

	if (ImGui::MenuItem("Edit")) {

	}

	if (ImGui::MenuItem("Windows")) {

	}

	ImGui::EndMainMenuBar();
}

void ModuleImGui::AddPanelToRenderList(DockPanel* newPanel)
{
	activeDockPanels.push_back(newPanel); 
}

void ModuleImGui::DrawPanels()
{
	for (auto it = activeDockPanels.begin(); it != activeDockPanels.end(); it++) {
		(*it)->Draw(); 
	}
}

update_status ModuleImGui::Update(float dt)
{
	DrawDockSpace(); 

	ImGui::StyleColorsDark(); 
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

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.31f, 0.25f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.74f, 0.74f, 0.94f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.68f, 0.68f, 0.68f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.62f, 0.70f, 0.72f, 0.56f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.33f, 0.14f, 0.47f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.97f, 0.31f, 0.13f, 0.81f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.42f, 0.75f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.65f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.40f, 0.62f, 0.80f, 0.15f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.39f, 0.64f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.67f, 0.80f, 0.59f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.48f, 0.53f, 0.67f);

	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.48f, 0.47f, 0.47f, 0.71f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.47f, 0.99f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.00f, 0.79f, 0.18f, 0.78f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.42f, 0.82f, 1.00f, 0.81f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.72f, 1.00f, 1.00f, 0.86f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.65f, 0.78f, 0.84f, 0.80f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.88f, 0.94f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.68f, 0.74f, 0.80f);//ImVec4(0.46f, 0.84f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.99f, 0.54f, 0.43f);

	style.Alpha = 1.0f;

	style.FrameRounding = 4;
	style.IndentSpacing = 12.0f;
	style.WindowPadding = ImVec2(2.5f, 2.5f); 
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

