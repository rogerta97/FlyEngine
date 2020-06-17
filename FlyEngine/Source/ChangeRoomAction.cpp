#include "ChangeRoomAction.h"
#include "imgui.h"
#include "FlyObject.h"
#include "GameInventory.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "ModuleWorldManager.h"

#include "mmgr.h"

ChangeRoomAction::ChangeRoomAction(FlyObject* _parentObject)
{
	actionType = ACTION_CHANGE_ROOM;
	isVisual = false;

	occ_SceneEnter = false;
	occ_SceneLeave = false;
	occ_ObjectClicked = false;
	occ_blackboardValue = false;
	occ_continuous = false;
	occ_mouseOver = false;

	SetActionName("Change Room");
	SetToolDescription("This should be the description of the scene change");
	parentObject = _parentObject; 
}

ChangeRoomAction::~ChangeRoomAction()
{
}

void ChangeRoomAction::DrawActionOccurenceCheckboxes()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Action Happens On:");
	ImGui::PopFont();

	ImGui::PushFont(App->moduleImGui->rudaRegularMid);
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.14f, 0.17f, 1.00f));

	static char itemToClickWithNameBuffer[256];
	int childSize = 95; 

	if (showClickWithItems)
		childSize += 30; 

	//if (showVariableConditions)
	//	childSize += 100;

	ImGui::BeginChild("##OccChild", ImVec2(ImGui::GetContentRegionAvailWidth(), childSize));
	INC_CURSOR_4;
	if (ImGui::Checkbox("Scene Enter", &occ_SceneEnter))
	{
	}

	INC_CURSOR_4;
	if (ImGui::Checkbox("Object Clicked", &occ_ObjectClicked))
	{
		if (!occ_ObjectClicked)
		{
			strcpy(itemToClickWithNameBuffer, "");
			itemToClickWith = nullptr;
			flog("item to click is null now"); 
			showClickWithItems = false;
		}
	}

	if (occ_ObjectClicked)
	{
		ImGui::SameLine();
		static std::string clickWithItemConditionButtonText = "Click With Item";
		if (ImGui::SmallButton(clickWithItemConditionButtonText.c_str()))
		{
			if (showClickWithItems)
			{
				showClickWithItems = false;
				clickWithItemConditionButtonText = "Click With Item";
			}
			else
			{
				showClickWithItems = true;
				clickWithItemConditionButtonText = "Hide";
			}
		}

		if (showClickWithItems)
		{
			if (itemToClickWith != nullptr)
			{
				strcpy(itemToClickWithNameBuffer, itemToClickWith->GetName().c_str()); 
			}

			INC_CURSOR_X_4;
			ImGui::InputTextWithHint("", "Item...", itemToClickWithNameBuffer, IM_ARRAYSIZE(itemToClickWithNameBuffer)); 

			ImGui::SameLine();
			if (ImGui::Button("Search##SearchInventoryItem"))
			{
				ImGui::OpenPopup("inventory_item_selection_popup"); 
			}
		}

		FlyObject* newInventoryItem = App->moduleWorldManager->PrintInventoryObjectSelectionPopup(); 

		if (newInventoryItem != nullptr)
		{
			itemToClickWith = newInventoryItem;
		}
	}

	INC_CURSOR_X_4;
	if (ImGui::Checkbox("Blackboard Condition", &occ_blackboardValue))
	{

	}

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


	ImGui::Spacing(); 
	ImGui::EndChild(); 

	if (showVariableConditions)
		DrawActionConditionsList();

	ImGui::PopFont();
	ImGui::PopStyleColor(); 
}

bool ChangeRoomAction::PassAllOccurrenceConditions()
{
	bool ret = true;
	// Object Clicked 
	if (occ_ObjectClicked && this->parentObject->GetClickableArea()->IsBoxClicked())
	{
		ret = false;
	}

	// Blackboard Conditions 
	if (occ_blackboardValue && !PassConditionTest())
		ret = false; 

	return ret; 
}

void ChangeRoomAction::Init()
{
}

void ChangeRoomAction::Update(float dt)
{
}

void ChangeRoomAction::CleanUp()
{
	Action::CleanUp();
}

void ChangeRoomAction::DrawSelectDestinationCombo()
{
	std::list<Room*> roomsInWorld = App->moduleWorldManager->GetRoomsInWorldList();

	char selectedRoom[256] = "Select Destination...";
	if (GetDesination() != nullptr)
	{
		strcpy(selectedRoom, GetDesination()->GetName().c_str());
	}

	if (ImGui::BeginCombo("Destination", selectedRoom))
	{
		int count = 0;
		for (auto& currentRoom : roomsInWorld)
		{
			if (parentObject->GetParentRoom() == nullptr)
				continue; 

			if (parentObject->GetParentRoom()->GetUID() == currentRoom->GetUID())
				continue; 

			if (ImGui::Selectable(currentRoom->GetName().c_str()))
			{
				SetDestination(currentRoom);
				parentObject->GetParentRoom()->AddConnection(currentRoom); 
			}
		}
		ImGui::EndCombo();
	}
}

void ChangeRoomAction::DrawUISettings()
{
	if (ImGui::CollapsingHeader("Change Room Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if(parentObject->flyObjectType == ACTION_OBJECT)
			DrawActionOccurenceCheckboxes();

		DrawSelectDestinationCombo();
	}
}

void ChangeRoomAction::DrawUISettingsInButton()
{
	ImGui::PushFont(App->moduleImGui->rudaBoldBig);
	ImGui::Text("Change Room Settings: ");
	ImGui::PopFont();

	DrawSelectDestinationCombo(); 
}

void ChangeRoomAction::SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr, int actionPositionInObject)
{
	string toolsSerializeSection;
	
	if (!literalStr)
		toolsSerializeSection = serializeObjectString + string("Actions.Action_") + to_string(actionPositionInObject) + ".";
	else
		toolsSerializeSection = serializeObjectString; 

	Action::SaveAction(jsonObject, toolsSerializeSection);
	Action::SaveOccurrence(jsonObject, toolsSerializeSection);

	if (destinationRoom != nullptr)
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), destinationRoom->GetName().c_str());
	else
		json_object_dotset_string(jsonObject, string(toolsSerializeSection + string("Destination")).c_str(), 0);
}

void ChangeRoomAction::DoAction()
{
	if (destinationRoom)
	{
		if (GameInventory::getInstance()->droppingObject != nullptr)
			GameInventory::getInstance()->returnDroppingObject = true; 

		GameInventory::getInstance()->CheckReturnDroppingObject();

		App->BroadCastEvent(FlyEngineEvent::LEAVE_ROOM);
		App->moduleWorldManager->SetSelectedRoom(destinationRoom);
		App->BroadCastEvent(FlyEngineEvent::ENTER_ROOM);
		SetActionCompleted(true); 
	}
}

void ChangeRoomAction::SetDestination(Room* dstRoom)
{
	destinationRoom = dstRoom; 
}

Room* ChangeRoomAction::GetDesination()
{
	return destinationRoom;
}
