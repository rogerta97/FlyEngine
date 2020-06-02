#include "RoomsNodeGraph.h"
#include "imgui.h"
#include "Texture.h"
#include "Application.h"
#include "ModuleManager.h"
#include "GameInventory.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "ModuleInput.h"
#include "ModuleWorldManager.h"
#include "ResourceManager.h"

RoomsNodeGraph::RoomsNodeGraph()
{
	roomManager = App->moduleWorldManager;
}

RoomsNodeGraph::~RoomsNodeGraph()
{
}

void RoomsNodeGraph::DrawGraph()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(0, 130, 189, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(76, 150, 185, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(0, 152, 223, 255));

	imnodes::BeginNodeEditor();

	// Draw Nodes
	if (roomManager->GetRoomsAmount() > 0)
	{
		// Draw Nodes 
		for (auto& currentRoom : roomManager->GetRoomsInWorldList())
		{
			if (currentRoom->isStart == true)
			{
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(189, 154, 0, 255));
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(185, 166, 76, 255));
				imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(223, 182, 0, 255));
			}

			imnodes::BeginNode(currentRoom->GetUID());

			imnodes::BeginNodeTitleBar();
			ImGui::PushFont(App->moduleImGui->rudaBlackMid);
			ImGui::Text("%s", currentRoom->GetName().c_str());
			ImGui::PopFont();
			imnodes::EndNodeTitleBar();

			// Get Room Viewport Texture 
			if (currentRoom->roomThumbnail == nullptr)
			{
				currentRoom->roomThumbnail = (Texture*)ResourceManager::getInstance()->GetResource("EmptyObject");
			}

			ImVec2 imageMaxSize = ImVec2(140, 140);
			ImVec2 uiImageDimensions = ImVec2(0, 0);

			if (currentRoom->roomThumbnail->IsVertical())
			{
				uiImageDimensions.y = imageMaxSize.y;
				uiImageDimensions.x = uiImageDimensions.y * currentRoom->roomThumbnail->GetAspectRatio();

				if (uiImageDimensions.x >= imageMaxSize.x)
				{
					float diff = uiImageDimensions.x - imageMaxSize.x;
					uiImageDimensions.x -= diff;
					uiImageDimensions.y = uiImageDimensions.x * currentRoom->roomThumbnail->GetAspectRatio();
				}
			}
			else
			{
				uiImageDimensions.x = imageMaxSize.x;
				uiImageDimensions.y = uiImageDimensions.x / currentRoom->roomThumbnail->GetAspectRatio();

				if (uiImageDimensions.y >= imageMaxSize.y)
				{
					float diff = uiImageDimensions.y - imageMaxSize.y;
					uiImageDimensions.y -= diff;
					uiImageDimensions.x = uiImageDimensions.y * currentRoom->roomThumbnail->GetAspectRatio();
				}
			}

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (70 - uiImageDimensions.x / 2));
			ImGui::Image((ImTextureID)currentRoom->roomThumbnail->GetTextureID(), uiImageDimensions);

			// Draw Enter Pin
			imnodes::BeginInputAttribute(currentRoom->GetUID() - 1);
			ImGui::Text("In");
			imnodes::EndAttribute();

			// Draw Output Pin
			imnodes::BeginOutputAttribute(currentRoom->GetUID() + 1);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 110);
			ImGui::Text("Out");
			imnodes::EndAttribute();

			imnodes::EndNode();

			if (currentRoom->isStart == true)
			{
				imnodes::PopColorStyle();
				imnodes::PopColorStyle();
				imnodes::PopColorStyle();
			}
		}

		// Draw Links
		int count = 0;
		for (auto& currentRoom : roomManager->GetRoomsInWorldList())
		{
			for (auto& currentLink : currentRoom->outLinks)
			{
				const std::pair<int, int> p(currentLink->originRoom->GetUID() + 1, currentLink->destinationRoom->GetUID() - 1);
				imnodes::Link(count, p.first, p.second);
				count++;
			}
		}
	}
	else
	{
		INC_CURSOR_X_7;
		ImGui::PushFont(App->moduleImGui->rudaBlackGiant);
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 0.30f), "The Project Is Empty");
		ImGui::PopFont();
	}

	imnodes::EndNodeEditor();

	// Handle Selected Nodes
	int num_selected_nodes = imnodes::NumSelectedNodes();
	if (App->moduleInput->GetMouseButton(RI_MOUSE_BUTTON_1_DOWN) && ImGui::IsItemHovered() && num_selected_nodes > 0)
	{
		std::vector<int> selected_nodes_id_list;
		selected_nodes_id_list.resize(num_selected_nodes);
		imnodes::GetSelectedNodes(selected_nodes_id_list.data());

		std::vector<int>::reverse_iterator currentNodeID = selected_nodes_id_list.rbegin();
		for (auto& currentNodeID : selected_nodes_id_list)
		{
			Room* selectedRoom = App->moduleWorldManager->GetRoom(currentNodeID);

			if (selectedRoom != nullptr)
			{
				App->moduleWorldManager->SetSelectedRoom(currentNodeID);
				num_selected_nodes = 0;
				return;
			}
		}

	}

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();

}

void RoomsNodeGraph::Update()
{
	// Check Input 
	if (App->moduleInput->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN)
	{
		ImGui::OpenPopup("DELETING ROOM - Are You Sure?");
	}

	if (ImGui::BeginPopupModal("DELETING ROOM - Are You Sure?", 0, ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowSize(ImVec2(330, 105));
		ImGui::PushFont(App->moduleImGui->rudaRegularTiny);
		ImGui::TextWrapped("You are about to delete a room, this action cannot be reversed and will be lost forever, are you sure?");
		ImGui::PopFont();

		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x / 2 - 50);
		if (ImGui::Button("Delete"))
		{
			Room* selectedRoom = App->moduleWorldManager->GetSelectedRoom();
			App->moduleWorldManager->DeleteRoom(selectedRoom);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("Keep"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
