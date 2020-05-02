#include "RoomsNodeGraph.h"
#include "imgui.h"
#include "Texture.h"
#include "Application.h"
#include "ModuleManager.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "ModuleRoomManager.h"
#include "ResourceManager.h"

RoomsNodeGraph::RoomsNodeGraph()
{
	roomManager = App->moduleRoomManager; 
}

RoomsNodeGraph::~RoomsNodeGraph()
{
}

void RoomsNodeGraph::DrawGraph()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(247, 200, 40, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(255, 80, 80, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(255, 80, 80, 255));

	imnodes::BeginNodeEditor();

	if (roomManager->GetRoomsAmount() > 0)
	{
		// Draw Nodes 
		for (auto& currentRoom : roomManager->GetRoomsInWorldList())
		{
			imnodes::BeginNode(currentRoom->GetUID());

			imnodes::BeginNodeTitleBar();
			ImGui::Text("%s", currentRoom->GetName().c_str());
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
			imnodes::BeginInputAttribute(currentRoom->GetUID() + 1);
			ImGui::Text("In");
			imnodes::EndAttribute();

			// Draw Output Pin
			imnodes::BeginOutputAttribute(currentRoom->GetUID() + 2);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 105);
			ImGui::Text("Out");
			imnodes::EndAttribute();

			imnodes::EndNode();

			// Draw Answer Pins
			//DrawAnswerPins(currentStep);
		}

		// Draw Links
		//DrawGraphLinks();
	}
	else
	{
		INC_CURSOR_X_7;
		ImGui::PushFont(App->moduleImGui->rudaBlackGiant);
		ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 0.25f), "No Rooms In World");
		ImGui::PopFont();
	}

	imnodes::EndNodeEditor();

	// Handle Selected Nodes
	int num_selected_nodes = imnodes::NumSelectedNodes();
	if (num_selected_nodes > 0)
	{
		std::vector<int> selected_nodes_id_list;
		selected_nodes_id_list.resize(num_selected_nodes);
		imnodes::GetSelectedNodes(selected_nodes_id_list.data());

		for (auto& selectedNodeID : selected_nodes_id_list)
		{
			Room* selectedRoom = App->moduleRoomManager->GetRoom(selectedNodeID);

			if (selectedRoom != nullptr)
			{
				App->moduleRoomManager->SetSelectedRoom(selectedNodeID); 
				num_selected_nodes = 0;
				return;
			}
		}
	}

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();

}
