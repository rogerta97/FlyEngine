#include "ActionConditionVariable.h"
#include "Action.h"
#include "FlyVariable.h"
#include "Room.h"
#include "Blackboard.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "Application.h"
#include "ModuleWorldManager.h"
#include "Texture.h"

#include "mmgr.h"

ActionConditionVariable::ActionConditionVariable()
{
	targetVariable = nullptr;
	actionConditionOperator = AC_EQUALS_TO;
	targetValueInteger = 0;
	targetValueBoolean = false;

	actionConditionType = CONDITION_IS_VARIABLE; 
}

ActionConditionVariable::~ActionConditionVariable()
{
}

void ActionConditionVariable::CleanUp()
{
	targetVariable = nullptr;
}

void ActionConditionVariable::SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos)
{
	std::string saveStr = serializeObjectString + "Condition_" + to_string(pos);

	json_object_dotset_number(jsonObject, std::string(saveStr + ".ConditionType").c_str(), actionConditionType);

	if(targetVariable != nullptr)
		json_object_dotset_string(jsonObject, std::string(saveStr + ".TargetVariableName").c_str(), targetVariable->name.c_str());
	else
		json_object_dotset_string(jsonObject, std::string(saveStr + ".TargetVariableName").c_str(), "None");

	json_object_dotset_number(jsonObject, std::string(saveStr + ".ConditionOperator").c_str(), actionConditionOperator);
	json_object_dotset_number(jsonObject, std::string(saveStr + ".TargetValueInteger").c_str(), targetValueInteger);
	json_object_dotset_boolean(jsonObject, std::string(saveStr + ".TargetValueBoolean").c_str(), targetValueBoolean);
}

void ActionConditionVariable::DrawUIItem(int itemPosition)
{
	//*******************************************************************************************
	// Draw Check Variable Icon
	//*******************************************************************************************
	INC_CURSOR_7;
	Texture* checkVariableIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckVariableIcon");
	ImGui::Image((ImTextureID)checkVariableIcon->GetTextureID(), ImVec2(35, 35));
	ImGui::SameLine();

	// Find button for target variable -----------------------------
	std::string findButtonID = "Find##FindVariableConditionButton" + to_string(itemPosition);

	INC_CURSOR_Y_7;
	Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
	if (ImGui::Button(findButtonID.c_str()))
	{
		ImGui::OpenPopup(string("search_variable_popup" + to_string(itemPosition)).c_str());
	}

	FlyVariable* selectedPopupVar = App->moduleWorldManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup(string("search_variable_popup" + to_string(itemPosition)).c_str());
	if (selectedPopupVar != nullptr)
	{
		targetVariable = selectedPopupVar;
	}

	//*******************************************************************************************
	// Draw InputText where the name of the object selected is going to be displayed 
	//*******************************************************************************************
	ImGui::SameLine();
	std::string inputTextID = "##InputTextCondition" + to_string(itemPosition);
	char varNameBuffer[256] = "";
	if (targetVariable != nullptr)
		strcpy(varNameBuffer, targetVariable->name.c_str());

	float itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.50f;
	float itemDesiredOffset = 0;

	ImGui::PushItemWidth(itemDesiredWidth + itemDesiredOffset);
	ImGui::InputTextWithHint(inputTextID.c_str(), "Target Variable...", varNameBuffer, IM_ARRAYSIZE(varNameBuffer));
	ImGui::PopItemWidth();

	int conditionOperatorType = actionConditionOperator;

	//*******************************************************************************************
	// Operators and Target Value 
	//*******************************************************************************************
	if (targetVariable != nullptr)
	{
		if (targetVariable->varType == Var_Integer)
		{
			std::string comboID = "##ComboTextCondition" + to_string(itemPosition);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemDesiredWidth - itemDesiredOffset / 2);
			if (ImGui::Combo(comboID.c_str(), &conditionOperatorType, "Equals To\0Greater Than\0Less Than\0"))
			{
				actionConditionOperator = (ActionConditionOperator)conditionOperatorType;
			}
			ImGui::PopItemWidth();

			std::string inputIntID = "##InputIntTarget" + to_string(itemPosition);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemDesiredWidth - itemDesiredOffset / 2);
			ImGui::InputInt(inputIntID.c_str(), &targetValueInteger);
			ImGui::PopItemWidth();

		}
		else if (targetVariable->varType == Var_Toggle)
		{
			std::string comboID = "##ComboTextBoolCondition" + to_string(itemPosition);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemDesiredWidth);
			if(ImGui::Combo(comboID.c_str(), &conditionOperatorType, "Equals To\0"))
			{
				actionConditionOperator = (ActionConditionOperator)conditionOperatorType;
			}

			ImGui::PopItemWidth();

			std::string inputBoolID = "Value##InputBoolTarget" + to_string(itemPosition);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemDesiredWidth);
			ImGui::Checkbox(inputBoolID.c_str(), &targetValueBoolean);
			ImGui::PopItemWidth();
		}
	}
}

bool ActionConditionVariable::PassTestCondition()
{
	bool ret = false; 
	switch (targetVariable->varType)
	{
	case Var_Integer:
		ret = PassTestIntegerVar(); 
		break; 

	case Var_Toggle:
		ret = PassTestToggleVar(); 
		break; 
	}

	return ret;
}

bool ActionConditionVariable::PassTestIntegerVar()
{
	switch (actionConditionOperator)
	{
	case AC_EQUALS_TO:
		if (targetVariable->varIntegerValue == targetValueInteger)
			return true; 
		break;

	case AC_BIGGER_THAN:
		if (targetVariable->varIntegerValue > targetValueInteger)
			return true;
		break;

	case AC_LESS_THAN:
		if (targetVariable->varIntegerValue < targetValueInteger)
			return true;
		break;

	case AC_DIFFERENT_THAN:
		if (targetVariable->varIntegerValue != targetValueInteger)
			return true;
		break;
	}

	return false;
}

bool ActionConditionVariable::PassTestToggleVar()
{
	switch (actionConditionOperator)
	{
	case AC_EQUALS_TO:
		if (targetVariable->varToogleValue == targetValueBoolean)
			return true;

		break;
	}
	return false;
}
