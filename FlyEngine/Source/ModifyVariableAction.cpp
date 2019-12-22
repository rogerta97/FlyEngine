#include "ModifyVariableAction.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Blackboard.h"
#include "Room.h"
#include "FlyVariable.h"

#include "imgui.h"

ModifyVariableAction::ModifyVariableAction(FlyObject* _parentObject)
{
	actionType = AT_MOD_VARIABLE;
	isVisual = false;

	SetActionName("Modify Variable");
	SetToolDescription("This should be the description of the modify variable action");
}

ModifyVariableAction::~ModifyVariableAction()
{
}

void ModifyVariableAction::DrawEffectVariablesUI()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));	
	ImGui::BeginChild("##ItemVarConatiner", ImVec2(ImGui::GetContentRegionAvailWidth(), 300));

	bool searchVarClicked = false; 

	int count = 0; 
	for (auto& currentEffect: variablesEffectList)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.28f, 0.43f, 0.56, 0.2f));
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 5, ImGui::GetCursorPos().y + 5));

		string childStr = "ChildItem" + to_string(count++); 
		ImGui::BeginChild(childStr.c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 5, 100));

		DrawEffectItem(currentEffect, count);

		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

	ImGui::EndChild();
	ImGui::PopStyleColor();
}

void ModifyVariableAction::DrawEffectItem(ModifyVariableEffect*& currentVariable, int pos)
{
	
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, ImGui::GetCursorPos().y + 8));

	// Name Var ----
	static char selectedVarName[256];
	strcpy(selectedVarName, currentVariable->targetVariable->name.c_str());

	string textNameStr = "##" + to_string(pos); 
	ImGui::InputTextWithHint(textNameStr.c_str(), "Target Variable...", selectedVarName, IM_ARRAYSIZE(selectedVarName));

	ImGui::SameLine();
	if (ImGui::Button("Search"))
	{
		ImGui::OpenPopup("search_variable_popup");
	}

	App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup();

	// Object Operator 
	if (currentVariable->targetVariable->varType == Var_Integer)
	{
		int intOperatorSelected = currentVariable->variableEffect;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##IntegerOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &intOperatorSelected, "Add\0Substract\0Set\0");

		currentVariable->variableEffect = (VariableEffect)intOperatorSelected; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
		ImGui::InputInt("Value", &currentVariable->targetVariable->varInteger);
	}
	else if (currentVariable->targetVariable->varType == Var_Toggle)
	{	
		int toggleOperatorSelected = currentVariable->variableEffect;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##ToggleOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &toggleOperatorSelected, "Set\0Toggle");

		currentVariable->variableEffect = (VariableEffect)toggleOperatorSelected;

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
		ImGui::Checkbox("Value", &currentVariable->targetVariable->varToogle);
	}
}


ModifyVariableEffect* ModifyVariableAction::AddEmptyEffect()
{
	ModifyVariableEffect* newEffect = new ModifyVariableEffect(); 

	FlyVariable* emptyVariable = new FlyVariable();
	emptyVariable->SetDefault(); 

	newEffect->targetVariable = emptyVariable; 
	newEffect->variableEffect = VarEffect_ADD; 

	variablesEffectList.push_back(newEffect); 

	return newEffect; 
}

int ModifyVariableAction::CountEffects()
{
	return variablesEffectList.size();
}

list<ModifyVariableEffect*>& ModifyVariableAction::GetEffectVariablesList()
{
	return variablesEffectList;
}
