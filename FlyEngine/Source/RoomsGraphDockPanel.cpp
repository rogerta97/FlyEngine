#include "Application.h"
#include "RoomsGraphDockPanel.h"
#include "imgui.h"

#include "NodeGraph.h"
#include "NodeGraphEz.h"

RoomsGraphDockPanel::RoomsGraphDockPanel(bool isVisible) : DockPanel("Rooms Graph", isVisible)
{
	flyEngineSection = FLY_SECTION_ROOM_GRAPH;
	dockPanelType = DOCK_ROOMS_GRAPH;
}

RoomsGraphDockPanel::~RoomsGraphDockPanel()
{
}

bool RoomsGraphDockPanel::Draw()
{
	
#pragma region secutiryChecks
	if (!DockPanel::Draw())
		return false;
#pragma endregion

	if (ImGui::Begin(panelName.c_str(), &visible)) {
		
		static ImNodes::CanvasState canvas;
		ImNodes::BeginCanvas(&canvas);

		struct Node
		{
			ImVec2 pos{};
			bool selected{};
			ImNodes::Ez::SlotInfo inputs[1];
			ImNodes::Ez::SlotInfo outputs[1];
		};

		static Node nodes[3] = {
			{{50, 100}, false, {{"Enter", 1}}, {{"Exit", 1}}},
			{{250, 50}, false, {{"Enter", 1}}, {{"Exit", 1}}},
			{{250, 100}, false, {{"Enter", 1}}, {{"Exit", 1}}},
		};

		for (Node& node : nodes)
		{
			if (ImNodes::Ez::BeginNode(&node, "Node Title", &node.pos, &node.selected))
			{
				ImNodes::Ez::InputSlots(node.inputs, 1);
				ImNodes::Ez::OutputSlots(node.outputs, 1);
				ImNodes::Ez::EndNode();
			}
		}

		ImNodes::Connection(&nodes[1], "Enter", &nodes[0], "Exit");
		ImNodes::Connection(&nodes[2], "Enter", &nodes[0], "Exit");

		ImNodes::EndCanvas();	

		//if (showGraph)
		//	ShowExampleAppCustomNodeGraph(&graphOpened);
	}

	ImGui::End();
}
