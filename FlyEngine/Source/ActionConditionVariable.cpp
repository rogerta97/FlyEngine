#include "ActionConditionVariable.h"
#include "Action.h"
#include "FlyVariable.h"
#include "Room.h"
#include "Blackboard.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "Application.h"
#include "ModuleRoomManager.h"
#include "Texture.h"

ActionConditionVariable::ActionConditionVariable()
{
	targetVariable = nullptr;
	actionConditionOperator = AC_EQUALS_TO;
	targetValueInteger = 0;
	targetValueBoolean = false;

	actionConditionType = AC_CHECK_VARIABLE; 
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
	json_object_dotset_string(jsonObject, std::string(saveStr + ".TargetVariableName").c_str(), targetVariable->name.c_str());

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_number(jsonObject, std::string(saveStr + ".ConditionOperator").c_str(), actionConditionOperator);

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_number(jsonObject, std::string(saveStr + ".TargetValueInteger").c_str(), targetValueInteger);

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_boolean(jsonObject, std::string(saveStr + ".TargetValueBoolean").c_str(), targetValueBoolean);
}

void ActionConditionVariable::DrawUIItem(int itemPosition)
{
	// Find button for target variable -----------------------------
	std::string findButtonID = "Find##FindButton" + to_string(itemPosition);

	if (itemPosition == 0)
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 3, ImGui::GetCursorPosY() + 4));
	else
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3);

	Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
	if (ImGui::Button(findButtonID.c_str()))
	{
		ImGui::OpenPopup("search_variable_popup");
	}

	//if (count == showSelectionPopup)
	//{
		FlyVariable* selectedPopupVar = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup();
		if (selectedPopupVar != nullptr)
		{
			targetVariable = selectedPopupVar;
			//showSelectionPopup = -1;
		}
	//}

	// Condition Variable Name -----------------------------------
	ImGui::SameLine();
	std::string inputTextID = "##InputTextCondition" + to_string(itemPosition);
	char varNameBuffer[256] = "";
	if (targetVariable != nullptr)
		strcpy(varNameBuffer, targetVariable->name.c_str());

	float itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.5f; 
	float itemDesiredOffset = 0;

	ImGui::PushItemWidth( + itemDesiredOffset);
	ImGui::InputTextWithHint(inputTextID.c_str(), "Target Variable...", varNameBuffer, IM_ARRAYSIZE(varNameBuffer));
	ImGui::PopItemWidth();

	int conditionOperatorType = actionConditionOperator;

	// Operators and Target Value ---------------------------------
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
			ImGui::Combo(comboID.c_str(), &conditionOperatorType, "Equals To\0");
			ImGui::PopItemWidth();

			std::string inputBoolID = "Value##InputBoolTarget" + to_string(itemPosition);
			ImGui::SameLine();
			ImGui::PushItemWidth(itemDesiredWidth);
			ImGui::Checkbox(inputBoolID.c_str(), &targetValueBoolean);
			ImGui::PopItemWidth();
		}
	}
}
