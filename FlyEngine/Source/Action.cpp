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
}

void Action::SaveAction(JSON_Object* jsonObject, std::string serializeObjectString)
{
}

void Action::DoAction()
{
	
}

void Action::DrawValueConditionsList()
{
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.17f, 0.19f, 0.22f, 1.00f));
	ImGui::BeginChild("valueConditions", ImVec2(ImGui::GetContentRegionAvailWidth(), 260), true);

	// Evalutation Criteria
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 8, ImGui::GetCursorPosY() + 4));
	ImGui::PushFont(App->moduleImGui->rudaBoldMid); 
	ImGui::Text("Evaluation Criteria:");
	ImGui::PopFont(); 

	static int evaluationCritatiaComboInt = 0; 
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::Combo("", &evaluationCritatiaComboInt, "All conditions must succed\0One condition must succed");

	ImGui::Spacing();
	ImGui::Separator(); 
	ImGui::Spacing();
		
	// Value Conditions List 
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::PushFont(App->moduleImGui->rudaBoldMid);
	ImGui::Text("Conditions List:");
	ImGui::PopFont();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));
	ImGui::BeginChild("valueConditionsHolder", ImVec2(ImGui::GetContentRegionAvailWidth() - 5, 100));

	// Iterate Conditions 
	int count = 0; 
	int itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.5f;
	int itemDesiredOffset = ImGui::GetContentRegionMax().x / 10; 
	for (auto& currentCondition : actionVariableConditions)
	{
		// Target Variable 
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 3, ImGui::GetCursorPosY() + 4));
		Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
		if (ImGui::ImageButton((ImTextureID)searchIcon->GetTextureID(), ImVec2(18, 18)))
		{
			ImGui::OpenPopup("search_variable_popup");
		}

		FlyVariable* selectedPopupVar = App->moduleRoomManager->GetSelectedRoom()->GetBlackboard()->DrawVariableListPopup();
		if (selectedPopupVar != nullptr)
		{
			currentCondition->targetVariable = selectedPopupVar; 
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
				if (ImGui::Combo(comboID.c_str(), &conditionOperatorType, "This\0Is\0A\0Test\0"))
				{
					currentCondition->actionConditionOperator = (ActionConditionOperator)conditionOperatorType;
				}
				ImGui::PopItemWidth();

				ImGui::SameLine();
				ImGui::PushItemWidth(itemDesiredWidth - itemDesiredOffset / 2);
				ImGui::InputInt("", &currentCondition->targetValueInteger); 
				ImGui::PopItemWidth();
			
			}
			else if (currentCondition->targetVariable->varType == Var_Toggle)
			{
				ImGui::SameLine();
				ImGui::PushItemWidth(90);
				ImGui::Combo("", &conditionOperatorType, "This\0Is\0A\0Bool\0Test\0");
				ImGui::PopItemWidth();

				ImGui::SameLine();
				ImGui::PushItemWidth(90);
				ImGui::Checkbox("", &currentCondition->targetValueBoolean);
				ImGui::PopItemWidth();
			}
		}

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

void Action::AddEmptyCondition()
{
	ActionCondition* newCondition = new ActionCondition(); 
	actionVariableConditions.push_back(newCondition); 
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
