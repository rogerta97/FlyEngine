#include "ActionConditionHasItem.h"
#include "Action.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "Application.h"
#include "Room.h"
#include "ModuleRoomManager.h"
#include "Texture.h"
#include "FlyObject.h"
#include "ModuleImGui.h"

#include "mmgr.h"

ActionConditionHasItem::ActionConditionHasItem()
{
	actionConditionType = CONDITION_HAS_ITEM;
}

ActionConditionHasItem::~ActionConditionHasItem()
{
	
}

void ActionConditionHasItem::CleanUp()
{
}

void ActionConditionHasItem::SaveCondition(JSON_Object* jsonObject, std::string serializeObjectString, int pos)
{
}

void ActionConditionHasItem::DrawUIItem(int itemPosition)
{
	//*******************************************************************************************
	//* Draw Check Inventory Item Icom
	//*******************************************************************************************
	INC_CURSOR_10;
	Texture* checkInventoryItemIcon = (Texture*)ResourceManager::getInstance()->GetResource("CheckInventoryItemIcon");
	ImGui::Image((ImTextureID)checkInventoryItemIcon->GetTextureID(), ImVec2(35, 35));
	ImGui::SameLine();

	//*******************************************************************************************
	//* Draw Find Item Inventory Button
	//*******************************************************************************************
	Room* selectedRoom = App->moduleRoomManager->GetSelectedRoom();
	std::string findButtonID = "Find##FindItemConditionButton" + to_string(itemPosition);

	INC_CURSOR_Y_10;
	Texture* searchIcon = (Texture*)ResourceManager::getInstance()->GetResource("SearchIcon");
	if (ImGui::Button(findButtonID.c_str()))
	{
		ImGui::OpenPopup("search_item_inv_popup");
	}

	if (ImGui::BeginPopup("search_item_inv_popup"))
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Inventory Items in %s Room", selectedRoom->GetName().c_str());
		ImGui::PopFont();

		ImGui::Separator(); 
		
		list<FlyObject*>* inventoryItems = &selectedRoom->GetInventoryItemsList();

		if (!inventoryItems->empty())
		{
			for (auto& currentItem : *inventoryItems)
			{
				ImGui::Selectable(currentItem->GetName().c_str());
			}
		}
		else
		{
			ImGui::Text("Empty"); 
		}

		ImGui::EndPopup(); 
	}

	//*******************************************************************************************
	//* Draw InputText where the name of the item selected is going to be displayed 
	//*******************************************************************************************
	ImGui::SameLine();
	std::string inputTextID = "##InputTextConditionHasItem" + to_string(itemPosition);
	char itemNameBuffer[256] = "";

	if (!item_to_check_name.empty())
		strcpy(itemNameBuffer, item_to_check_name.c_str());

	float itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.5f;
	float itemDesiredOffset = 0;

	ImGui::PushItemWidth(itemDesiredWidth + itemDesiredOffset);
	ImGui::InputTextWithHint(inputTextID.c_str(), "Which item..?", itemNameBuffer, IM_ARRAYSIZE(itemNameBuffer));
	ImGui::PopItemWidth();
}
