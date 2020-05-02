#include "RoomsNodeGraph.h"
#include "imgui.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "Room.h"
#include "ModuleRoomManager.h"

RoomsNodeGraph::RoomsNodeGraph()
{
	roomManager = App->moduleRoomManager; 
}

RoomsNodeGraph::~RoomsNodeGraph()
{
}

void RoomsNodeGraph::DrawGraph()
{
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(247, 40, 40, 255));
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
			uint thumbnailTexture = 0;

			if (currentRoom->roomTextureID != 0)
			{
				thumbnailTexture = currentRoom->roomTextureID;
			}

			ImGui::Image((ImTextureID)thumbnailTexture, ImVec2(133, 100));

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

	imnodes::PopColorStyle();
	imnodes::PopColorStyle();
	imnodes::PopColorStyle();

}
