#include "ActionConditionHasItem.h"
#include "Action.h"
#include "imgui.h"
#include "ResourceManager.h"
#include "Application.h"
#include "Room.h"
#include "ModuleRoomManager.h"
#include "Texture.h"
#include "GameInventory.h"
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
	string serializeStr = serializeObjectString + "Condition_" + to_string(pos); 

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
		ImGui::OpenPopup(string("search_item_inv_popup" + to_string(itemPosition)).c_str());
	}

	if (ImGui::BeginPopup(string("search_item_inv_popup" + to_string(itemPosition)).c_str()))
	{
		ImGui::PushFont(App->moduleImGui->rudaBoldBig);
		ImGui::Text("Inventory Items in %s:", selectedRoom->GetName().c_str());
		ImGui::PopFont();

		ImGui::Separator(); 
		
		list<FlyObject*> inventoryItems = selectedRoom->GetInventoryItemsList();
	
		if (!inventoryItems.empty())
		{
			for (auto& currentItem : inventoryItems)
			{
				Texture* inventoryItemIcon = (Texture*)ResourceManager::getInstance()->GetResource("InventoryItemIcon");
				ImGui::Image((ImTextureID)inventoryItemIcon->GetTextureID(), ImVec2(20, 20));
				ImGui::SameLine();

				if (ImGui::Selectable(currentItem->GetName().c_str()))
				{
					itemToCheckName = currentItem->GetName().c_str(); 
					itemToCheckUID = currentItem->GetUID(); 
				}
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

	if (!itemToCheckName.empty())
		strcpy(itemNameBuffer, itemToCheckName.c_str());

	float itemDesiredWidth = ImGui::GetContentRegionMax().x / 3.5f;
	float itemDesiredOffset = 0;

	ImGui::PushItemWidth(itemDesiredWidth + itemDesiredOffset);
	ImGui::InputTextWithHint(inputTextID.c_str(), "Which item..?", itemNameBuffer, IM_ARRAYSIZE(itemNameBuffer));
	ImGui::PopItemWidth();
}

bool ActionConditionHasItem::PassTestCondition()
{
	if(GameInventory::getInstance()->IsItemInInventory(itemToCheckUID))
		return true;

	return false; 
}
