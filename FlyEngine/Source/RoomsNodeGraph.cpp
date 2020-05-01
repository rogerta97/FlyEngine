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
	/*imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(59, 131, 255, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_LinkHovered, IM_COL32(255, 173, 51, 255));
*/
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

			ImGui::Image(0, ImVec2(65, 50));

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
		ImGui::TextColored(ImVec4(1, 0.5f, 0.5f, 0.25f), "No dialogue selected");
		ImGui::PopFont();
	}

	imnodes::EndNodeEditor();

	//imnodes::PopColorStyle();
	//imnodes::PopColorStyle();

}
