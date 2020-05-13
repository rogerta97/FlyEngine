#include "GlobalBlackboardDockPanel.h"
#include "imgui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ModuleWorldManager.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "FlyVariable.h"

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
			App->moduleWorldManager->globalBlackboard->AddDefaultVariable();
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

		DrawBlackboardVariables();
	}

	ImGui::End();

	return true;
}

void GlobalBlackboardDockPanel::DrawBlackboardVariables()
{
	Blackboard* globalBB = App->moduleWorldManager->globalBlackboard;

	PUSH_CHILD_BG_COLOR;
	ImGui::BeginChild("BlackboardChild", ImVec2(ImGui::GetContentRegionMax().x - 5, ImGui::GetContentRegionAvail().y));

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, (ImGui::GetCursorPos().y + 5)));

	int counter = 0;
	for (auto& currentVar : globalBB->blackboardVariablesList)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.28f, 0.43f, 0.56, 0.2f));

		string varChildID = "VariableUIGroup" + to_string(counter);
		ImGui::BeginChild(varChildID.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 5, 95));

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 70);

		int iconSize = 47;
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, (ImGui::GetContentRegionAvail().y / 2) - iconSize / 2));
		Texture* variableType = nullptr;

		if (currentVar->varType == Var_Integer)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
		else if (currentVar->varType == Var_Toggle)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

		ImGui::Image((ImTextureID*)variableType->GetTextureID(), ImVec2(iconSize, iconSize));

		ImGui::NextColumn();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);

		char nameBuffer[256];
		strcpy(nameBuffer, currentVar->name.c_str());
		string nameStringID = "Name##VarName" + to_string(counter);

		if (ImGui::InputText(nameStringID.c_str(), nameBuffer, IM_ARRAYSIZE(nameBuffer)))
		{
			currentVar->name = nameBuffer;
		}

		string comboStringID = "Variable Type##VarType" + to_string(counter);
		int currentItemType = currentVar->varType;
		ImGui::Combo(comboStringID.c_str(), &currentItemType, "Integer\0Toggle\0");

		string valueStringID = "Value##ComboVar" + to_string(counter);
		switch (currentItemType)
		{

		case 0:
		{
			ImGui::InputInt(valueStringID.c_str(), &currentVar->varIntegerValue);
			currentVar->varType = Var_Integer;

			break;
		}

		case 1:
		{
			ImGui::Checkbox(valueStringID.c_str(), &currentVar->varToogleValue);
			currentVar->varType = Var_Toggle;
			break;
		}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();

		counter++;
	}

	ImGui::PopStyleColor();

	ImGui::EndChild();
}
