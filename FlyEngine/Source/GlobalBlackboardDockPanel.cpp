#include "GlobalBlackboardDockPanel.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ModuleWorldManager.h"
#include "ModuleImGui.h"
#include "Room.h"

#include "Globals.h"

GlobalBlackboardDockPanel::GlobalBlackboardDockPanel(bool isVisible) : DockPanel("Global Blackboard", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_GLOBAL_BLACKBOARD;
}

GlobalBlackboardDockPanel::~GlobalBlackboardDockPanel()
{
}

bool GlobalBlackboardDockPanel::Draw()
{
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible))
	{
		// Add and delete variable Buttons -----------
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);

		Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
		if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(30, 30)))
		{
			App->moduleWorldManager->GetSelectedRoom()->GetBlackboard()->AddDefaultVariable();
		}

		ImGui::SameLine();
		Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("MinusIconWhite");
		if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(30, 30)))
		{

		}

		ImGui::PopStyleVar();
		IMGUI_SPACED_SEPARATOR;

		// Search Bar --------------------
		static char searchVariableBuffer[256];
		ImGui::InputTextWithHint("", "Search...", searchVariableBuffer, IM_ARRAYSIZE(searchVariableBuffer));

		// Child List Space Title --------
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Blackboard Variables:");
		POP_FONT;
	}

	ImGui::End();

	return true;
}
