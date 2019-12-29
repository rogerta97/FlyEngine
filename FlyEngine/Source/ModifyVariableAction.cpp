#include "ModifyVariableAction.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Blackboard.h"
#include "Room.h"
#include "Texture.h"
#include "FlyVariable.h"
#include "ResourceManager.h"

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

void ModifyVariableAction::DrawEffectItem(ModifyVariableEffect*& modifyVarEffect, int pos)
{
	ImGui::Columns(2); 

	ImGui::SetColumnWidth(0, 60);

	int operatorTextureID = 0;
	Texture* operatorTexture = nullptr; 

	switch (modifyVarEffect->variableEffect)
	{

	case VariableEffect::VarEffect_ADD:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("AddOperatorIcon");
		operatorTextureID = operatorTexture->GetTextureID(); 
		break; 

	case VariableEffect::VarEffect_SUBSTRACT:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("SubstractOperatorIcon");
		operatorTextureID = operatorTexture->GetTextureID();
		break;

	case VariableEffect::VarEffect_SET_NUMBER:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("EqualOperatorIcon");
		operatorTextureID = operatorTexture->GetTextureID();
		break;
	}

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 7, ImGui::GetContentRegionMax().y / 2 - 25)); 
	ImGui::Image((ImTextureID)operatorTextureID, ImVec2(45, 45)); 

	ImGui::NextColumn(); 

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, ImGui::GetCursorPos().y + 8));

	// Name Var ----
	static char selectedVarName[256];
	strcpy(selectedVarName, modifyVarEffect->targetVariable->name.c_str());

	string textNameStr = "##" + to_string(pos); 
	ImGui::InputTextWithHint(textNameStr.c_str(), "Target Variable...", selectedVarName, IM_ARRAYSIZE(selectedVarName));

	ImGui::SameLine();
	if (ImGui::Button("Search"))
	{
		ImGui::OpenPopup("search_variable_popup");
	}

	FlyVariable* popupVarSelected = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup();
	if (popupVarSelected != nullptr)
	{
		modifyVarEffect->targetVariable = popupVarSelected; 
	}

	// Object Operator 
	if (modifyVarEffect->targetVariable->varType == Var_Integer)
	{
		int intOperatorSelected = modifyVarEffect->variableEffect;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##IntegerOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &intOperatorSelected, "Add\0Substract\0Set\0");

		modifyVarEffect->variableEffect = (VariableEffect)intOperatorSelected; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
		ImGui::InputInt("Value", &modifyVarEffect->targetVariable->varInteger);
	}
	else if (modifyVarEffect->targetVariable->varType == Var_Toggle)
	{	
		int toggleOperatorSelectedInt = modifyVarEffect->variableEffect;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##ToggleOperator" + to_string(pos);
		if (ImGui::Combo(textNameStr.c_str(), &toggleOperatorSelectedInt, "\0Toggle\0Set"))

		modifyVarEffect->variableEffect = (VariableEffect)(toggleOperatorSelectedInt + 2);
	
		if (modifyVarEffect->variableEffect != VariableEffect::VarEffect_TOGGLE)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
			ImGui::Checkbox("Value", &modifyVarEffect->targetVariable->varToogle);
		}
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
