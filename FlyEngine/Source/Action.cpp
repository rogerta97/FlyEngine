#include "Action.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Room.h"
#include "ModuleRoomManager.h"
#include "FlyVariable.h"

#include "mmgr.h"

Action::Action()
{
	actionType = AT_null; 
	isSelected = false; 

	occ_SceneEnter = false;
	occ_SceneLeave = false;
	occ_blackboardValue = false;
	occ_ObjectClicked = false;

	return; 
}

Action::~Action()
{

}

void Action::Init()
{
}

void Action::Update()
{
}

void Action::Draw()
{
}

void Action::CleanUp()
{
	for (auto& currentCondition : actionVariableConditions)
	{
		currentCondition->CleanUp();
		delete currentCondition; 
	}
}

void Action::DrawUISettings()
{
}

void Action::DrawUISettingsInButton()
{
}

void Action::SaveAction(JSON_Object* jsonObject, std::string serializeObjectString, bool literalString)
{
	json_object_dotset_number(jsonObject, string(serializeObjectString + "ActionType").c_str(), actionType);
	SaveActionConditions(serializeObjectString, jsonObject);
}

void Action::SaveOccurrence(JSON_Object* jsonObject, string serializeObjectString)
{
	string serializeObjectOccurrenceStr = serializeObjectString + "Occurrence.";

	json_object_dotset_boolean(jsonObject, string(serializeObjectOccurrenceStr + "SceneEnter").c_str(), IsOccSceneEnter());
	json_object_dotset_boolean(jsonObject, string(serializeObjectOccurrenceStr + "SceneLeave").c_str(), IsOccSceneLeave());
	json_object_dotset_boolean(jsonObject, string(serializeObjectOccurrenceStr + "ObjectClicked").c_str(), IsOccObjectClicked());
	json_object_dotset_boolean(jsonObject, string(serializeObjectOccurrenceStr + "BlackboardCondition").c_str(), IsOccBlackboardValue());
}

void Action::SaveActionConditions(std::string& serializeObjectString, JSON_Object* jsonObject)
{
	int conditionsAmount = this->actionVariableConditions.size(); 
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Conditions.ConditionsAmount").c_str(), conditionsAmount);

	if (!actionVariableConditions.empty())
	{
		string conditionsSaveStr = serializeObjectString + "Conditions."; 
		int count = 0;
		for (auto& currentCondition : actionVariableConditions)
		{
			currentCondition->SaveCondition(jsonObject, conditionsSaveStr, count++);
		}
	}
}

void Action::LoadOccurrence(JSON_Object* jsonObject, string serializeObjectString)
{
	occ_SceneEnter = json_object_dotget_boolean(jsonObject, string(serializeObjectString + "SceneEnter").c_str());
	occ_SceneLeave = json_object_dotget_boolean(jsonObject, string(serializeObjectString + "SceneLeave").c_str());
	occ_blackboardValue = json_object_dotget_boolean(jsonObject, string(serializeObjectString + "BlackboardCondition").c_str());
	occ_ObjectClicked = json_object_dotget_boolean(jsonObject, string(serializeObjectString + "ObjectClicked").c_str());
}

void Action::DoAction()
{
	
}

void Action::DrawValueConditionsList()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.17f, 0.19f, 0.22f, 1.00f));
	ImGui::BeginChild("valueConditions", ImVec2(ImGui::GetContentRegionAvailWidth(), 310), true);

	// Evalutation Criteria
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 8, ImGui::GetCursorPosY() + 4));
	ImGui::PushFont(App->moduleImGui->rudaBoldMid); 
	ImGui::Text("Evaluation Criteria:");
	ImGui::PopFont(); 

	int evaluationCritatiaComboInt = (int)evaluationCriteria; 
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	if (ImGui::Combo("", &evaluationCritatiaComboInt, "All conditions must succed\0One condition must succed"))
	{
		switch (evaluationCritatiaComboInt)
		{
		case 0:
			evaluationCriteria = ALL_SUCCED; 
			break; 

		case 1: 
			evaluationCriteria = ONE_SUCCED;
			break; 
		}
	}

	ImGui::Spacing();
	ImGui::Separator(); 
	ImGui::Spacing();
		
	// Value Conditions List 
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::Text("Conditions List:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("valueConditionsHolder", ImVec2(ImGui::GetContentRegionAvailWidth() - 30, 150));

	// Iterate Conditions 
	int count = 0; 
	static int showSelectionPopup = -1;
	int itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.5f;
	int itemDesiredOffset = 0; 
	for (auto& currentCondition : actionVariableConditions)
	{
		// Target Variable 
		std::string findButtonID = "Find##FindButton" + to_string(count);

		if (count == 0)
			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 3, ImGui::GetCursorPosY() + 4));
		else
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3); 

		Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		if (ImGui::Button(findButtonID.c_str()))
		{
			ImGui::OpenPopup("search_variable_popup");
			showSelectionPopup = count; 
		}

		if (count == showSelectionPopup)
		{
			FlyVariable* selectedPopupVar = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup();
			if (selectedPopupVar != nullptr)
			{
				currentCondition->targetVariable = selectedPopupVar; 
				showSelectionPopup = -1; 
			}
		}

		ImGui::SameLine(); 
		std::string inputTextID = "##InputTextCondition" + to_string(count); 
		char varNameBuffer[256] = ""; 
		if (currentCondition->targetVariable != nullptr)
			strcpy(varNameBuffer, currentCondition->targetVariable->name.c_str()); 
		
		ImGui::PushItemWidth(itemDesiredWidth + itemDesiredOffset);
		ImGui::InputTextWithHint(inputTextID.c_str(), "Target Variable...", varNameBuffer, IM_ARRAYSIZE(varNameBuffer));
		ImGui::PopItemWidth();
		
		int conditionOperatorType = currentCondition->actionConditionOperator;
		
		// Operators and Target Value 
		if (currentCondition->targetVariable != nullptr)
		{
			if (currentCondition->targetVariable->varType == Var_Integer)
			{
				std::string comboID = "##ComboTextCondition" + to_string(count);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemDesiredWidth - itemDesiredOffset / 2);
				if (ImGui::Combo(comboID.c_str(), &conditionOperatorType, "Equals To\0Greater Than\0Less Than\0"))
				{
					currentCondition->actionConditionOperator = (ActionConditionOperator)conditionOperatorType;
				}
				ImGui::PopItemWidth();

				std::string inputIntID = "##InputIntTarget" + to_string(count);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemDesiredWidth - itemDesiredOffset / 2);
				ImGui::InputInt(inputIntID.c_str(), &currentCondition->targetValueInteger);
				ImGui::PopItemWidth();
			
			}
			else if (currentCondition->targetVariable->varType == Var_Toggle)
			{
				std::string comboID = "##ComboTextBoolCondition" + to_string(count);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemDesiredWidth);
				ImGui::Combo(comboID.c_str(), &conditionOperatorType, "Equals To\0");
				ImGui::PopItemWidth();

				std::string inputBoolID = "Value##InputBoolTarget" + to_string(count);
				ImGui::SameLine();
				ImGui::PushItemWidth(itemDesiredWidth);
				ImGui::Checkbox(inputBoolID.c_str(), &currentCondition->targetValueBoolean);
				ImGui::PopItemWidth();
			}
		}

		if(count < actionVariableConditions.size() - 1)
			ImGui::Separator();

		count++; 
	}

	ImGui::EndChild();
	ImGui::PopStyleColor(); 

	ImGui::Spacing();
	ImGui::Separator(); 

	// Draw Condition Buttons 
	Texture* plusTexture = (Texture*)ResourceManager::getInstance()->GetResource("PlusIcon"); 
	if (ImGui::ImageButton((ImTextureID)plusTexture->GetTextureID(), ImVec2(25, 25))) 
	{
		AddEmptyCondition(); 
	}

	ImGui::SameLine(); 
	Texture* minusTexture = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
	if (ImGui::ImageButton((ImTextureID)minusTexture->GetTextureID(), ImVec2(25, 25)))
	{

	}

	ImGui::EndChild(); 
	ImGui::PopStyleColor(); 
}

std::string Action::GetActionName() const
{
	return toolName;
}

void Action::SetActionName(std::string newName)
{
	toolName = newName;
}

void Action::LoadConditions(JSON_Object* jsonObject, string serializeObjectString)
{
	string conditionsStr = serializeObjectString + ".Conditions"; 
	
}

bool& Action::IsOccSceneEnter()
{
	return occ_SceneEnter; 
}

bool& Action::IsOccSceneLeave()
{
	return occ_SceneLeave;
}

bool& Action::IsOccBlackboardValue()
{
	return occ_blackboardValue;
}

bool& Action::IsOccObjectClicked()
{
	return occ_ObjectClicked;
}

void Action::SetOccBlackboardValue(bool newValue)
{
	occ_blackboardValue = newValue; 
}

void Action::SetOccSceneEnter(bool newOccSceneEnter)
{
	occ_SceneEnter = newOccSceneEnter; 
}

void Action::SetOccSceneLeave(bool newOccSceneLeave)
{
	occ_SceneLeave = newOccSceneLeave;
}

void Action::SetOccObjectClicked(bool newOccObjectClicked)
{
	occ_ObjectClicked = newOccObjectClicked;
}

ActionCondition* Action::AddEmptyCondition()
{
	ActionCondition* newCondition = new ActionCondition(); 
	actionVariableConditions.push_back(newCondition); 
	return newCondition;
}

FlyObject* Action::GetParentObject() const
{
	return parentObject;
}

void Action::SetParentObject(FlyObject* newParentObject)
{
	parentObject = newParentObject; 
}

std::string Action::GetToolDescription() const
{
	return toolDescription;
}

void Action::SetToolDescription(std::string newDescription)
{
	toolDescription = newDescription; 
}

ActionType Action::GetActionType() const
{
	return actionType;
}

void Action::SetToolType(ActionType newToolType)
{
	actionType = newToolType; 
}

bool& Action::IsSelected()
{
	return isSelected;
}

void Action::SetSelected(bool newSelected)
{
	isSelected = newSelected; 
}

bool& Action::HasVisual()
{
	return isVisual; 
}

ActionSelectableInfo Action::GetActionSelectableInfo()
{
	ActionSelectableInfo returnToolInfo; 

	returnToolInfo.actionName = toolName; 
	returnToolInfo.actionDescription = toolDescription; 
	returnToolInfo.actionType = actionType; 

	return returnToolInfo;
}

ActionCondition::ActionCondition()
{
	targetVariable = nullptr;
	actionConditionOperator = AC_EQUALS_TO; 
	targetValueInteger = 0;
	targetValueBoolean = false; 
}

ActionCondition::~ActionCondition()
{
}

void ActionCondition::CleanUp()
{
	targetVariable = nullptr; 
}

void ActionCondition::SaveCondition(JSON_Object* jsonObject, string serializeObjectString, int pos)
{
	std::string saveStr = serializeObjectString + "Condition_" + to_string(pos); 
	json_object_dotset_string(jsonObject, string(saveStr + ".TargetVariableName").c_str(), targetVariable->name.c_str()); 

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_number(jsonObject, string(saveStr + ".ConditionOperator").c_str(), actionConditionOperator);

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_number(jsonObject, string(saveStr + ".TargetValueInteger").c_str(), targetValueInteger);

	saveStr = serializeObjectString + "Condition_" + to_string(pos);
	json_object_dotset_boolean(jsonObject, string(saveStr + ".TargetValueBoolean").c_str(), targetValueBoolean);
}
