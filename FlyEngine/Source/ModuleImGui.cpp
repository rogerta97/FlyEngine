#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleRoomManager.h"
#include "Application.h"

#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "ConsoleDockPanel.h"
#include "RoomsGraphDockPanel.h"
#include "ViewportManager.h"
#include "TextureMSAA.h"
#include "WorldPropertiesDockPanel.h"
#include "GraphPropertiesDockPanel.h"
#include "ObjectCreatorDockPanel.h"
#include "MyFileSystem.h"
#include "OpenGL.h"

#include "GameViewportDockPanel.h"
#include "RoomObjectsDockPanel.h"
#include "SceneDockPanel.h"
#include "ObjectPropertiesDockPanel.h"

#include "Room.h"
#include "FlyObject.h"

#include <string>
#include <Windows.h>
#include "mmgr.h"

// cherry colors, 3 intensities
#define HI(v)   ImVec4(0.502f, 0.075f, 0.256f, v)
#define MED(v)  ImVec4(0.455f, 0.198f, 0.301f, v)
#define LOW(v)  ImVec4(0.232f, 0.201f, 0.271f, v)
// backgrounds (@todo: complete with BG_MED, BG_LOW)
#define BG(v)   ImVec4(0.200f, 0.220f, 0.270f, v)
// text
#define TEXT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

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
	string fontsDirectory = MyFileSystem::getInstance()->GetSolutionDirectory() + "EngineResources/Fonts/Exo-Regular.otf";

	fontsDirectory = MyFileSystem::getInstance()->GetSolutionDirectory() + "EngineResources/Fonts/";

	rudaRegularMid = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Regular.ttf").c_str(), 19);
	rudaBoldMid = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Bold.ttf").c_str(), 19);
	rudaBlackMid = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Black.ttf").c_str(), 19);

	rudaRegularSmall = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Regular.ttf").c_str(), 15);
	rudaBoldSmall = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Bold.ttf").c_str(), 15);
	rudaBlackSmall = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Black.ttf").c_str(), 15);

	rudaRegularBig = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Regular.ttf").c_str(), 22);
	rudaBoldBig = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Bold.ttf").c_str(), 22);
	rudaBlackBig = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Black.ttf").c_str(), 22);

	rudaRegularHuge = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Regular.ttf").c_str(), 30);
	rudaBoldHuge = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Bold.ttf").c_str(), 30);
	rudaBlackHuge = io.Fonts->AddFontFromFileTTF(string(fontsDirectory + "Ruda-Black.ttf").c_str(), 30);

	CreatePanels();
	SetStyle();

	AddaptToFlySection(FLY_SECTION_ROOM_EDIT); 

	FlyObject* parrot = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject("Transformer Yellow");
	string spritePath = MyFileSystem::getInstance()->GetSolutionDirectory() + "EngineResources\\Images\\TransformerYellow.png";
	parrot->AddImageTool(spritePath.c_str());

	parrot->CreateClickableArea(float2(0, 0), float2(1, 1));
	parrot->clickableAreaActive = true; 

	//FlyObject* test = App->moduleRoomManager->GetSelectedRoom()->CreateFlyObject("Transformer");
	//spritePath = MyFileSystem::getInstance()->GetSolutionDirectory() + "EngineResources/Images/Transformer.png"; 
	//test->AddImageTool(spritePath.c_str());

	ViewportManager::getInstance();

	return true; 
}

void ModuleImGui::CreatePanels()
{
	ConsoleDockPanel* consolePanel = new ConsoleDockPanel(true);
	RoomsGraphDockPanel* roomsGraphPanel = new RoomsGraphDockPanel(true);
	GraphPropertiesDockPanel* graphPropertiesPanel = new GraphPropertiesDockPanel(true);
	WorldPropertiesDockPanel* roomPropertiesPanel = new WorldPropertiesDockPanel(true);

	GameViewportDockPanel* gameViewporPanel = new GameViewportDockPanel(false);
	ObjectCreatorDockPanel* objectCreatorPanel = new ObjectCreatorDockPanel(false); 
	RoomObjectsDockPanel* roomObjectsDockPanel = new RoomObjectsDockPanel(false);
	ObjectPropertiesDockPanel* objectPropertiesDockPanel = new ObjectPropertiesDockPanel(false);
	SceneDockPanel* sceneDockPanel = new SceneDockPanel(false); 

	dockPanels.push_back(consolePanel); 
	dockPanels.push_back(roomsGraphPanel);
	dockPanels.push_back(graphPropertiesPanel);
	dockPanels.push_back(roomPropertiesPanel); 

	dockPanels.push_back(gameViewporPanel);
	dockPanels.push_back(objectCreatorPanel); 
	dockPanels.push_back(roomObjectsDockPanel);
	dockPanels.push_back(objectPropertiesDockPanel);
	dockPanels.push_back(sceneDockPanel);

	consoleDockPanel = consolePanel; 
	graphPropertiesDockPanel = graphPropertiesPanel;
	objectCreatorDockPanel = objectCreatorPanel; 
	gameViewportDockPanel = gameViewporPanel;
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

	if (App->flySection == FLY_SECTION_ROOM_EDIT) {
		if (ImGui::SmallButton("Back To World")) {
			App->moduleImGui->AddaptToFlySection(FLY_SECTION_ROOM_GRAPH); 
		}
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
	if (flyEngineSection == FlyEngineSection::FLY_SECTION_ROOM_EDIT && App->moduleRoomManager->GetSelectedRoom() == nullptr) {
		FLY_ERROR("Room Editor can not open with any room selected"); 
	}

	for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {

		if ((*it)->GetFlySection() == flyEngineSection || (*it)->GetFlySection() == FLY_SECTION_BOTH)
			(*it)->SetVisible(true);
		else
			(*it)->SetVisible(false); 
	}

	if(flyEngineSection == FLY_SECTION_ROOM_EDIT || flyEngineSection == FLY_SECTION_ROOM_GRAPH)
		App->flySection = flyEngineSection;
}

DockPanel* ModuleImGui::GetDockPanel(DockPanelType panelType)
{
	for (auto it = dockPanels.begin(); it != dockPanels.end(); it++) {
		
		if ((*it)->GetPanelType() == panelType)
			return (*it);
	}
}

update_status ModuleImGui::PostUpdate(float dt)
{
	DrawDockSpace();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

void ModuleImGui::ReceiveEvent(FlyEngineEvent eventType)
{
	for (auto& it : dockPanels) {
		(it)->ReceiveEvent(eventType); 
	}
}

bool ModuleImGui::CleanUp()
{
	DeletePanels(); 
	NodeGraph::getInstance()->DeleteAllConnections();
	NodeGraph::getInstance()->DeleteAllNodes();

	ImGui_ImplSDL2_Shutdown(); 

	return true;
}

void ModuleImGui::SetStyle() 
{
	ImGui::GetStyle().FrameRounding = 4.0f;
	ImGui::GetStyle().GrabRounding = 4.0f;
	ImGui::GetStyle().ItemSpacing = ImVec2(5.0f, 5.0f);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_Border] = ImVec4(0.27f, 0.40f, 0.53f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.28f, 0.35f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.44f, 0.56f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.21f, 0.24f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.31f, 0.33f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.30f, 0.41f, 0.57f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.29f, 0.39f, 0.47f, 0.55f);
	colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.44f, 0.58f, 1.00f);
}

void ModuleImGui::DarkTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4* colors = ImGui::GetStyle().Colors;
	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.08f, 0.09f, 0.58f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.55f, 0.21f, 0.28f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.49f, 0.25f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.89f, 0.71f, 0.77f, 0.80f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.76f, 0.44f, 0.57f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(1.00f, 0.84f, 0.86f, 0.19f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.66f, 0.37f, 0.37f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
}

void ModuleImGui::CherryTheme()
{

	ImGuiStyle* style = &ImGui::GetStyle();

	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(7.0f, 5.0f);
	style->ItemInnerSpacing = ImVec2(7.0f, 5.0f);
	style->WindowPadding = ImVec2(9.0f, 9.0f);
	style->FramePadding = ImVec2(2.0f, 2.0f); 
	style->IndentSpacing = 30;
	style->ScrollbarSize = 15; 

	style->WindowRounding = 4;
	style->ChildRounding = 4;
	style->FrameRounding = 2; 
	style->PopupRounding = 2; 
	style->GrabRounding = 4; 


	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.50f, 0.18f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.54f, 0.33f, 0.42f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = TEXT(0.28f);
	style->Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.19f, 0.20f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.00f, 0.00f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = BG(1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style->Colors[ImGuiCol_FrameBgActive] = MED(1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = HI(1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style->Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style->Colors[ImGuiCol_ScrollbarBg] = BG(1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style->Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style->Colors[ImGuiCol_ButtonActive] = MED(1.00f);
	style->Colors[ImGuiCol_Header] = MED(0.76f);
	style->Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style->Colors[ImGuiCol_HeaderActive] = HI(1.00f);
	style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.09f, 0.10f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.08f, 0.09f, 0.58f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.49f, 0.25f, 0.35f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.89f, 0.71f, 0.77f, 0.80f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.76f, 0.44f, 0.57f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.66f, 0.37f, 0.37f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style->Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style->Colors[ImGuiCol_ResizeGripActive] = MED(1.00f);
	style->Colors[ImGuiCol_PlotLines] = TEXT(0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = MED(1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = TEXT(0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = MED(1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.5f);

	// [...]
	style->Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.5f);
}