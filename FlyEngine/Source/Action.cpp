#include "Action.h"
#include "FlyObject.h"

#include "Application.h"
#include "ModuleImGui.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ActionCondition.h"
#include "Room.h"
#include "ModuleRoomManager.h"
#include "FlyVariable.h"
#include "ActionConditionVariable.h"
#include "ActionConditionHasItem.h"
#include "ActionCondition.h"

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

void Action::Update(float dt)
{
}

void Action::Draw()
{
}

void Action::CleanUp()
{
	for (auto& currentCondition : actionConditions)
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
	json_object_dotset_boolean(jsonObject, string(serializeObjectOccurrenceStr + "BlackboardCondition").c_str(), IsOccCondition());
}

void Action::SaveActionConditions(std::string& serializeObjectString, JSON_Object* jsonObject)
{
	int conditionsAmount = this->actionConditions.size(); 
	json_object_dotset_number(jsonObject, string(serializeObjectString + "Conditions.ConditionsAmount").c_str(), conditionsAmount);

	if (!actionConditions.empty())
	{
		string conditionsSaveStr = serializeObjectString + "Conditions."; 
		int count = 0;
		for (auto& currentCondition : actionConditions)
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

void Action::DrawActionConditionsList()
{
	// Evalutation Criteria
	ImGui::PushFont(App->moduleImGui->rudaBoldBig); 
	ImGui::Text("Evaluation Criteria:");
	ImGui::PopFont(); 

	int evaluationCritatiaComboInt = (int)evaluationCriteria; 
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

	// Value Conditions List 
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Conditions List:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("valueConditionsHolder", ImVec2(ImGui::GetContentRegionAvailWidth(), 250));

	int count = 0; 
	for (auto& currentCondition : actionConditions)
	{		
		currentCondition->DrawUIItem(count);
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
		ImGui::OpenPopup("SelectConditionType");
	}

	// Callback for when button Add is pressed
	OnAddConditionButtonPressed();

	ImGui::SameLine(); 
	Texture* minusTexture = (Texture*)ResourceManager::getInstance()->GetResource("MinusIcon");
	if (ImGui::ImageButton((ImTextureID)minusTexture->GetTextureID(), ImVec2(25, 25)))
	{

	}
}

void Action::OnAddConditionButtonPressed()
{
	if (ImGui::BeginPopup("SelectConditionType"))
	{		
		Texture* checkVariableIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckVariableIcon");
		ImGui::Image((ImTextureID)checkVariableIcon->GetTextureID(), ImVec2(18, 18));
		ImGui::SameLine();

		if (ImGui::Selectable("Check Blackboard Variable"))		
			AddEmptyCondition(CONDITION_IS_VARIABLE); 
		
		Texture* checkInventoryItemIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckInventoryItemIcon");
		ImGui::Image((ImTextureID)checkInventoryItemIcon->GetTextureID(), ImVec2(18, 18));
		ImGui::SameLine();

		if (ImGui::Selectable("Check Inventory Object"))
			AddEmptyCondition(CONDITION_HAS_ITEM);
		ImGui::EndPopup();
	}
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
	int conditionsAmount = json_object_dotget_number(jsonObject, string(serializeObjectString + "ConditionsAmount").c_str()); 

	int count = 0; 
	while (count < conditionsAmount)
	{
		string serializeStr = serializeObjectString + "Condition_" + to_string(count); 
		
		int conditionType_tmp = json_object_dotget_number(jsonObject, string(serializeStr + ".ConditionType").c_str()); 
		ActionConditionType actionConditionType = (ActionConditionType)conditionType_tmp; 
		ActionCondition* newCondition = nullptr; 

		switch (actionConditionType)
		{
		case CONDITION_IS_VARIABLE:
			newCondition = LoadConditionVariable(jsonObject, serializeStr, parentObject->GetParentRoom()->GetBlackboard());
			break;

		case CONDITION_HAS_ITEM:
			newCondition = LoadConditionHasItem(jsonObject, serializeStr);
			break; 
		}

		count++; 
	}	
}

ActionConditionHasItem* Action::LoadConditionHasItem(JSON_Object* jsonObject, string serializeObjectString)
{
	ActionConditionHasItem* newConditionHasItem = new ActionConditionHasItem(); 
	newConditionHasItem->itemToCheckUID = json_object_dotget_number(jsonObject, string(serializeObjectString + ".ItemToCheckUID").c_str());
	newConditionHasItem->itemToCheckName = json_object_dotget_string(jsonObject, string(serializeObjectString + ".ItemToCheckName").c_str());
	actionConditions.push_back(newConditionHasItem);
	return newConditionHasItem;
}

ActionConditionVariable* Action::LoadConditionVariable(JSON_Object* jsonObject, string serializeObjectString, Blackboard* currentBlackboard)
{
	ActionConditionVariable* newConditionVariable = new ActionConditionVariable();

	string targetVariableName = json_object_dotget_string(jsonObject, string(serializeObjectString + ".TargetVariableName").c_str());
	newConditionVariable->targetVariable = currentBlackboard->GetVariable(targetVariableName);

	int conditionOperator_tmp = json_object_dotget_number(jsonObject, string(serializeObjectString + ".ConditionType").c_str());
	newConditionVariable->actionConditionOperator = (ActionConditionOperator)conditionOperator_tmp;

	newConditionVariable->targetValueInteger = json_object_dotget_number(jsonObject, string(serializeObjectString + ".TargetValueInteger").c_str());
	newConditionVariable->targetValueBoolean = json_object_dotget_boolean(jsonObject, string(serializeObjectString + ".TargetValueBoolean").c_str());

	actionConditions.push_back(newConditionVariable);
	return newConditionVariable; 
}

bool Action::PassConditionTest()
{
	for (auto& currentCondition : actionConditions)
	{
		bool testResult = currentCondition->PassTestCondition();

		if (testResult == true)
		{
			if(evaluationCriteria == ONE_SUCCED)
				return true;	
		}
		else
		{
			if (evaluationCriteria == ALL_SUCCED)
				return false; 
		}
	}

	return true;
}

bool& Action::IsOccSceneEnter()
{
	return occ_SceneEnter; 
}

bool& Action::IsOccSceneLeave()
{
	return occ_SceneLeave;
}

bool& Action::IsOccCondition()
{
	return occ_blackboardValue;
}

bool& Action::IsOccMouseOver()
{
	return occ_mouseOver;
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

ActionCondition* Action::AddEmptyCondition(ActionConditionType conditionType)
{
	switch (conditionType)
	{
	case CONDITION_IS_VARIABLE:
	{
		ActionConditionVariable* conditionCheckVar = new ActionConditionVariable(); 
		actionConditions.push_back(conditionCheckVar);
		return conditionCheckVar; 
	}
	case CONDITION_HAS_ITEM:
	{
		ActionConditionHasItem* conditionHasItem = new ActionConditionHasItem();
		actionConditions.push_back(conditionHasItem);
		return conditionHasItem;
	}
	case AC_NONE:
		break;
	}

	return nullptr; 
}

void Action::AddCondition(ActionCondition* newCondition)
{
	actionConditions.push_back(newCondition); 
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

void Action::SetIsSelected(bool _isSelected)
{
	isSelected = _isSelected; 
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

bool Action::IsActionFinished()
{
	return actionFinished;
}

void Action::SetActionCompleted(bool isFinished)
{
	if (isFinished == actionFinished)
		return; 

	actionFinished = isFinished; 
	flog("%s finished", this->GetActionName().c_str()); 
}


