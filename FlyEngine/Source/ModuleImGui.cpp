#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_dock.h"
#include "imgui_impl_sdl.h"

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
	ImGui_ImplSdlGL2_Init(App->moduleWindow->window);
	ImGui::StyleColorsDark(); 

	//ImGuiIO& io = ImGui::GetIO();

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
	ImGui_ImplSdlGL2_NewFrame(App->moduleWindow->window);
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	ImGui::BeginDockspace();

	//ImGui::SetWindowPos(ImVec2(-5, offset));
	//ImGui::SetWindowSize(ImVec2(App->window->GetWidth() + 8, App->window->GetHeight() - offset));

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(App->moduleWindow->GetWidth() + 8, App->moduleWindow->GetHeight()));

	//ImGui::DockSpace(ImGui::GetID("MainDock"));

	//if (show_style_editor)
	//{
	//	if (ImGui::Begin("Style Editor", &show_style_editor))
	//	{
	//		ImGui::ShowStyleEditor();
	//	}
	//	ImGui::End();
	//}

	ImGui::BeginDock("First FlyEngine Window :D");
	ImGui::EndDock();

	ImGui::EndDockspace(); 


	DrawTopBar();
	
	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();
	return true;
}

void ModuleImGui::DrawTopBar() {

	ImGui::BeginMainMenuBar(); 

	ImGui::EndMainMenuBar(); 
}



