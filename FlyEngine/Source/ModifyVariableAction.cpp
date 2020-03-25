#include "ModifyVariableAction.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Blackboard.h"
#include "Room.h"
#include "Globals.h"
#include "Texture.h"
#include "ModuleImGui.h"
#include "FlyVariable.h"
#include "ResourceManager.h"
#include "ModuleManager.h"

#include "imgui.h"
#include "mmgr/mmgr.h"

ModifyVariableAction::ModifyVariableAction(FlyObject* _parentObject)
{
	actionType = ACTION_MOD_VARIABLE;
	isVisual = false;
	acceptSequencial = true;

	SetActionName("Modify Variable");
	SetToolDescription("This should be the description of the modify variable action");
}

ModifyVariableAction::~ModifyVariableAction()
{
}

void ModifyVariableAction::CleanUp()
{
	Action::CleanUp(); 

	for (auto& currentVariableEffect : variablesEffectList)
	{
		currentVariableEffect->CleanUp(); 
		delete currentVariableEffect;
		currentVariableEffect = nullptr; 
	}

	variablesEffectList.clear(); 
	parentObject = nullptr;
}

void ModifyVariableAction::DoAction()
{
	for (auto& currentEffect : variablesEffectList)
	{
		// Get Correct Blackboard (For Now Room) 
		Blackboard* roomBlackboard = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard(); 

		if (currentEffect->targetVariable->varType == Var_Integer)
		{
			roomBlackboard->ModifyIntegerVariable(currentEffect); 
		}
	}

	if (!variablesEffectList.empty())
	{
		SetActionCompleted(true); 
	}
}

void ModifyVariableAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection;

	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeObjectString;

	Action::SaveAction(jsonObject, toolsSerializeSection); 
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	json_object_dotset_number(jsonObject, string(toolsSerializeSection + "EffectsAmount").c_str(), variablesEffectList.size());

	int count = 0; 
	for (auto& currentEffect : variablesEffectList)
	{
		currentEffect->SaveEffect(jsonObject, toolsSerializeSection, count++);
	}
}

void ModifyVariableAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Modify Variable Attributes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Object Occurrence ---------
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Action Happens On:");
		ImGui::PopFont();

		DrawActionOccurenceCheckboxes();

		IMGUI_SPACED_SEPARATOR;

		// Object Settings ----------
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Variables To Modify:");
		ImGui::PopFont();

		DrawEffectVariablesUI();

		Texture* plusIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon");
		if (ImGui::ImageButton((ImTextureID)plusIcon->GetTextureID(), ImVec2(30, 30)))
		{
			AddEmptyEffect();
		}

		ImGui::SameLine();
		Texture* minusIcon = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
		if (ImGui::ImageButton((ImTextureID)minusIcon->GetTextureID(), ImVec2(30, 30)))
		{

		}
	}
}

void ModifyVariableAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	if (GetActionClass() == ACTION_CLASS_SEQUENTIAL)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 35));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Object Condition", &occ_blackboardValue);
	}
	else if (GetActionClass() == ACTION_CLASS_DIRECT)
	{
		ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));
		ImGui::SetCursorPos(ImVec2(5, 5));
		ImGui::Checkbox("Object Clicked", &occ_ObjectClicked);
		ImGui::SetCursorPos(ImVec2(5, 35));
		ImGui::Checkbox("Blackboard Value Condition", &IsOccCondition());
	}

	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), 70));

	ImGui::SetCursorPos(ImVec2(5, 8));
	ImGui::Checkbox("Object Clicked", &IsOccObjectClicked());
	ImGui::SetCursorPos(ImVec2(5, 38));
	ImGui::Checkbox("Blackboard Value Condition", &IsOccCondition());

	ImGui::SameLine();
	static std::string showValueConditionButtonText = "Show Conditions";
	if (ImGui::Button(showValueConditionButtonText.c_str()))
	{
		if (showVariableConditions)
		{
			showVariableConditions = false;
			showValueConditionButtonText = "Show Conditions";
		}
		else
		{
			showVariableConditions = true;
			showValueConditionButtonText = "Hide Conditions";
		}
	}

	ImGui::EndChild();

	if (showVariableConditions)
		DrawActionConditionsList();

	POP_FONT;
	ImGui::PopStyleColor();
}

void ModifyVariableAction::DrawUISettingsInButton()
{	
	// Object Settings ----------
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Variables To Modify:");
	ImGui::PopFont();

	DrawEffectVariablesUI();

	Texture* plusIcon = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon");
	if (ImGui::ImageButton((ImTextureID)plusIcon->GetTextureID(), ImVec2(30, 30)))
	{
		AddEmptyEffect();
	}

	ImGui::SameLine();
	Texture* minusIcon = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
	if (ImGui::ImageButton((ImTextureID)minusIcon->GetTextureID(), ImVec2(30, 30)))
	{

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

	int operatorTextureID = GetOperatorTextureIDFromType(modifyVarEffect->variableOperatorType);

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 7, ImGui::GetContentRegionMax().y / 2 - 25)); 
	ImGui::Image((ImTextureID)operatorTextureID, ImVec2(45, 45)); 

	ImGui::NextColumn(); 

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 8, ImGui::GetCursorPos().y + 8));

	// Name Var ----
	static char selectedVarName[256];
	strcpy(selectedVarName, modifyVarEffect->targetVariable->name.c_str());

	string textNameStr = "##" + to_string(pos); 
	ImGui::InputTextWithHint(textNameStr.c_str(), "Target Variable...", selectedVarName, IM_ARRAYSIZE(selectedVarName), ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	if (ImGui::Button("Search"))
	{
		ImGui::OpenPopup(string("search_variable_popup" + to_string(pos)).c_str());
	}

	FlyVariable* popupVarSelected = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup(string("search_variable_popup" + to_string(pos)).c_str());
	if (popupVarSelected != nullptr)
	{
		modifyVarEffect->variableOperatorType = (VariableOperatorType)0;
		modifyVarEffect->targetVariable = popupVarSelected; 
	}

	// Object Operator 
	if (modifyVarEffect->targetVariable->varType == Var_Integer)
	{
		int intOperatorSelected = modifyVarEffect->variableOperatorType;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##IntegerOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &intOperatorSelected, "Add\0Substract\0Set\0");

		modifyVarEffect->variableOperatorType = (VariableOperatorType)intOperatorSelected; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
		ImGui::InputInt("Value", &modifyVarEffect->incIntegerValue);
	}
	else if (modifyVarEffect->targetVariable->varType == Var_Toggle)
	{	
		int toggleOperatorSelectedInt = modifyVarEffect->variableOperatorType;

		if (toggleOperatorSelectedInt == VarEffect_TOGGLE) toggleOperatorSelectedInt = 0;
		else if (toggleOperatorSelectedInt == VarEffect_SET_TOGGLE) toggleOperatorSelectedInt = 1; 

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);

		string textNameStr = "Operator##ToggleOperator" + to_string(pos);
		ImGui::Combo(textNameStr.c_str(), &toggleOperatorSelectedInt, "Toggle\0Set");

		modifyVarEffect->variableOperatorType = (VariableOperatorType)(toggleOperatorSelectedInt + 3);
	
		if (modifyVarEffect->variableOperatorType != VariableOperatorType::VarEffect_TOGGLE)
		{
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
			ImGui::Checkbox("Value", &modifyVarEffect->nextToggleValue);
		}
	}
}


ModifyVariableEffect* ModifyVariableAction::AddEmptyEffect()
{
	ModifyVariableEffect* newEffect = new ModifyVariableEffect(); 
	newEffect->targetVariable = App->moduleManager->fakeVarInitAttach;
	variablesEffectList.push_back(newEffect); 
	return newEffect; 
}

void ModifyVariableAction::AddEffect(ModifyVariableEffect* newEffect)
{
	variablesEffectList.push_back(newEffect); 
}

int ModifyVariableAction::CountEffects()
{
	return variablesEffectList.size();
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
}

void ModifyVariableEffect::SaveEffect(JSON_Object* jsonObject, string serializeObjectString, int pos)
{
	serializeObjectString += "EffectsGroup.Effect_" + to_string(pos);
	std::string saveString = serializeObjectString + ".TargetVariableName";

	if (targetVariable != nullptr)
		json_object_dotset_string(jsonObject, saveString.c_str(), targetVariable->name.c_str());
	else
		json_object_dotset_string(jsonObject, saveString.c_str(), "..None..");

	saveString = serializeObjectString + ".OperatorType";
	json_object_dotset_number(jsonObject, saveString.c_str(), variableOperatorType);

	saveString = serializeObjectString + ".IncIntegerValue";
	json_object_dotset_number(jsonObject, saveString.c_str(), incIntegerValue);

	saveString = serializeObjectString + ".NextToggleValue";
	json_object_dotset_boolean(jsonObject, saveString.c_str(), nextToggleValue);

}

void ModifyVariableEffect::ApplyEffect()
{
	switch (variableOperatorType)
	{
	case VarEffect_ADD: 
		targetVariable->varIntegerValue += incIntegerValue; 
		break; 

	case VarEffect_SUBSTRACT:
		targetVariable->varIntegerValue -= incIntegerValue;
		break;

	case VarEffect_SET_NUMBER:
		targetVariable->varIntegerValue = incIntegerValue;
		break;

	case VarEffect_TOGGLE:
		targetVariable->varToogleValue != targetVariable->varToogleValue;
		break;

	case VarEffect_SET_TOGGLE:
		targetVariable->varToogleValue = nextToggleValue;
		break;
	}

	App->moduleManager->NotifyVariableChange(targetVariable); 
}

void ModifyVariableEffect::AttachToVariable(FlyVariable* _targetVariable)
{
	targetVariable = _targetVariable; 
}

ModifyVariableEffect::ModifyVariableEffect()
{
	incIntegerValue = 0;
	this->nextToggleValue = false; 
	this->variableOperatorType = VariableOperatorType::VarEffect_None; 
	targetVariable = nullptr; 
}
