#include "ModuleImGui.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"

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

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->moduleWindow->window);
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	ImGui::Begin("Yo no soy don omar, pero soy bandolero");
	ImGui::End();

	ImGui::Render();

	return update_status::UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();
	return true;
}

