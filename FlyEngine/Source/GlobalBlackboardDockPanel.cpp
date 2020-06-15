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
	flyEngineSection = FLY_SECTION_WORLD_EDITOR;
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
		//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
		//ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);



	//	ImGui::PopStyleVar();

		// Child List Space Title --------
		ImGui::PushFont(App->moduleImGui->rudaBlackBig);
		ImGui::Text("Blackboard Variables:");
		POP_FONT;

		// Search Bar --------------------
		static char searchVariableBuffer[256];
		ImGui::InputTextWithHint("", "Search...", searchVariableBuffer, IM_ARRAYSIZE(searchVariableBuffer));

		ImGui::SameLine();
		Texture* plusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("PlusIconWhite");
		if (ImGui::ImageButton((ImTextureID)plusIconTex->GetTextureID(), ImVec2(20, 20)))
		{
			App->moduleWorldManager->globalBlackboard->AddDefaultVariable();
		}

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

	int counter = 0;
	for (auto& currentVar : globalBB->blackboardVariablesList)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, (ImGui::GetCursorPos().y + 5)));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.28f, 0.43f, 0.56, 0.2f));

		string varChildID = "VariableUIGroup" + to_string(counter);
		ImGui::BeginChild(varChildID.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 5, 103));

		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, 70);

		int iconSize = 55;
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 4, (ImGui::GetContentRegionAvail().y / 2) - iconSize / 2));
		Texture* variableType = nullptr;

		if (currentVar->varType == Var_Integer)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("NaturalNumberIcon");
		else if (currentVar->varType == Var_Toggle)
			variableType = (Texture*)ResourceManager::getInstance()->GetResource("ToggleIcon");

		ImGui::Image((ImTextureID*)variableType->GetTextureID(), ImVec2(iconSize, iconSize));

		ImGui::NextColumn();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - 28);
		Texture* minusIconTex = (Texture*)ResourceManager::getInstance()->GetResource("DeleteIcon");

		if (ImGui::ImageButton((ImTextureID)minusIconTex->GetTextureID(), ImVec2(20, 20)))
		{

		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 27);

		char nameBuffer[256];
		strcpy(nameBuffer, currentVar->name.c_str());
		string nameStringID = "Name##VarName" + to_string(counter);

		if (ImGui::InputTextWithHint(nameStringID.c_str(), "Attach Variable...", nameBuffer, IM_ARRAYSIZE(nameBuffer)))
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
