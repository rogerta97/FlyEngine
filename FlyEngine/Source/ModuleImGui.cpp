#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "SDL_opengl.h"

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

	char* str = (char*)glGetString(GL_VERSION); 
	printf("GL Version: %s", str); 

	ImGui::StyleColorsDark(); 

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable; 

	//wchar_t buffer[MAX_PATH];
	//GetModuleFileName(NULL, buffer, MAX_PATH);
	//std::wstring ws(buffer);
	//std::string exeDir(ws.begin(), ws.end());

	//printf("Path: %s", str.c_str());
	////for (int i = 0; i < MAX_PATH; i++) {

	////	if(buffer[i] == '\0')
	////		break; 
	////		
	////}
	
	/*ImFont* font1 = io.Fonts->AddFontDefault();
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("C:/Users/FULLMAC/Documents/FlyEngine/FlyEngine/EngineResources/Fonts/Antonio-Regular.ttf", 16.0f);*/

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->moduleWindow->mainWindow);
	ImGui::NewFrame();
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	bool opened = true;

	ImGui::ShowDemoWindow(); 


	DrawTopBar();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSDL2_Shutdown(); 
	return true;
}

void ModuleImGui::DrawTopBar() {

	ImGui::BeginMainMenuBar(); 

	ImGui::EndMainMenuBar(); 
}



