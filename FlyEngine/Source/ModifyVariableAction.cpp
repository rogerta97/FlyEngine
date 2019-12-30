#include "ModifyVariableAction.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Blackboard.h"
#include "Room.h"
#include "Texture.h"
#include "FlyVariable.h"
#include "ResourceManager.h"

#include "imgui.h"
#include "mmgr.h"

ModifyVariableAction::ModifyVariableAction(FlyObject* _parentObject)
{
	actionType = AT_MOD_VARIABLE;
	isVisual = false;

	fakeVarInitAttach = new FlyVariable();
	fakeVarInitAttach->SetDefault();

	SetActionName("Modify Variable");
	SetToolDescription("This should be the description of the modify variable action");
}

ModifyVariableAction::~ModifyVariableAction()
{
}

void ModifyVariableAction::CleanUp()
{
	for (auto& currentVariableEffect : variablesEffectList)
	{
		currentVariableEffect->CleanUp(); 
		delete currentVariableEffect;
		currentVariableEffect = nullptr; 
	}

	variablesEffectList.clear(); 

	fakeVarInitAttach->CleanUp();
	delete fakeVarInitAttach; 
}

void ModifyVariableAction::DoAction()
{
	for (auto& currentEffect : variablesEffectList)
	{
		// Get Correct Blackboard (For Now Room) 
		Blackboard* roomBlackboard = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard(); 

		if (currentEffect->targetVariable->varType == Var_Integer)
			roomBlackboard->ModifyIntegerVariable(currentEffect); 
	}
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

	int operatorTextureID = GetOperatorTextureIDFromType(modifyVarEffect->variableEffect->variableOperatorType);

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
		modifyVarEffect->variableEffect->variableOperatorType = (VariableOperatorType)0;
		modifyVarEffect->targetVariable = popupVarSelected; 
	}

	// Object Operator 
	if (modifyVarEffect->targetVariable->varType == Var_Integer)
	{
		int intOperatorSelected = modifyVarEffect->variableEffect->variableOperatorType;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##IntegerOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &intOperatorSelected, "Add\0Substract\0Set\0");

		modifyVarEffect->variableEffect->variableOperatorType = (VariableOperatorType)intOperatorSelected; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
		ImGui::InputInt("Value", &modifyVarEffect->variableEffect->incIntegerValue);
	}
	else if (modifyVarEffect->targetVariable->varType == Var_Toggle)
	{	
		int toggleOperatorSelectedInt = modifyVarEffect->variableEffect->variableOperatorType;

		if (toggleOperatorSelectedInt == VarEffect_TOGGLE) toggleOperatorSelectedInt = 0;
		else if (toggleOperatorSelectedInt == VarEffect_SET_TOGGLE) toggleOperatorSelectedInt = 1; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##ToggleOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &toggleOperatorSelectedInt, "Toggle\0Set");

		modifyVarEffect->variableEffect->variableOperatorType = (VariableOperatorType)(toggleOperatorSelectedInt + 3);
	
		if (modifyVarEffect->variableEffect->variableOperatorType != VariableOperatorType::VarEffect_TOGGLE)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
			ImGui::Checkbox("Value", &modifyVarEffect->variableEffect->nextToggleValue);
		}
	}
}


ModifyVariableEffect* ModifyVariableAction::AddEmptyEffect()
{
	ModifyVariableEffect* newEffect = new ModifyVariableEffect(); 

	newEffect->targetVariable = fakeVarInitAttach;	

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

int ModifyVariableAction::GetOperatorTextureIDFromType(VariableOperatorType effectType)
{
	Texture* operatorTexture = nullptr;

	switch (effectType)
	{

	case VariableOperatorType::VarEffect_ADD:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("AddOperatorIcon");
		return operatorTexture->GetTextureID();

	case VariableOperatorType::VarEffect_SUBSTRACT:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("SubstractOperatorIcon");
		return operatorTexture->GetTextureID();

	case VariableOperatorType::VarEffect_SET_NUMBER:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("EqualOperatorIcon");
		return operatorTexture->GetTextureID();

	case VariableOperatorType::VarEffect_TOGGLE:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("ToggleOperatorIcon");
		return operatorTexture->GetTextureID();

	case VariableOperatorType::VarEffect_SET_TOGGLE:
		operatorTexture = (Texture*)ResourceManager::getInstance()->GetResource("EqualOperatorIcon");
		return operatorTexture->GetTextureID();
	}

	return 0; 
}

ModifyVariableEffect::~ModifyVariableEffect()
{
}

void ModifyVariableEffect::CleanUp()
{
	targetVariable = nullptr; 
	delete variableEffect;
	variableEffect = nullptr; 
}

void ModifyVariableEffect::ApplyEffect()
{
	switch (variableEffect->variableOperatorType)
	{
	case VarEffect_ADD: 
		targetVariable->varIntegerValue += variableEffect->incIntegerValue; 
		break; 

	case VarEffect_SUBSTRACT:
		targetVariable->varIntegerValue -= variableEffect->incIntegerValue;
		break;

	case VarEffect_SET_NUMBER:
		targetVariable->varIntegerValue = variableEffect->incIntegerValue;
		break;

	case VarEffect_TOGGLE:
		targetVariable->varToogleValue != targetVariable->varToogleValue;
		break;

	case VarEffect_SET_TOGGLE:
		targetVariable->varToogleValue = variableEffect->nextToggleValue;
		break;
	}
}

void ModifyVariableEffect::AttachToVariable(FlyVariable* _targetVariable)
{
	targetVariable = _targetVariable; 
}

ModifyVariableEffect::ModifyVariableEffect()
{
	variableEffect = new VariableEffect(); 
	targetVariable = nullptr; 
}

VariableEffect::~VariableEffect()
{
}

VariableEffect::VariableEffect()
{
	variableOperatorType = VarEffect_ADD; 

	incIntegerValue = 0; 
	nextToggleValue = false; 
}
