#include "Application.h"
#include "RoomsGraphDockPanel.h"
#include "imgui.h"
#include "SDL_opengl.h"

#include "ImNode.h"
#include "ImNodeEz.h"

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

		//GLuint my_opengl_texture;
		//glGenTextures(1, &my_opengl_texture);
		//// [...] load image, render to texture, etc.
		//ImGui::Image((void*)(intptr_t)my_opengl_texture, ImVec2(512, 512));


		//struct Node
		//{

		//	void Init(ImVec2& pos, bool selected, string title, ImNodes::Ez::SlotInfo* inputSlotInfo, ImNodes::Ez::SlotInfo* outputSlotInfo) {

		//	}

		//	// Node Draw Data
		//	ImVec2 pos;
		//	bool selected;
		//	string title; 

		//	// Node Content 
		//	GLuint roomImage;

		//	// Input & Ouput Slots
		//	ImNodes::Ez::SlotInfo inputs[1];
		//	ImNodes::Ez::SlotInfo outputs[1];
		//};


		//static Node nodes[3] = {
		//	{"Main Forest", {50, 100}, false, {{"", 1}}, {{"", 1}}},
		//	{"Room 01", {250, 50}, false, {{"", 1}}, {{"", 1}}},
		//	{"Room 02", {250, 100}, false, {{"", 1}}, {{"", 1}}},
		//};

		//for (Node& node : nodes)
		//{
		//	if (ImNodes::Ez::BeginNode(&node, node.title.c_str(), &node.pos, &node.selected))
		//	{
		//		ImNodes::Ez::InputSlots(node.inputs, 1);
		//		ImNodes::Ez::OutputSlots(node.outputs, 1);
		//		ImNodes::Ez::EndNode();
		//	}
		//}

		//ImNodes::Connection(&nodes[1], "", &nodes[0], "");
		//ImNodes::Connection(&nodes[2], "", &nodes[0], "");

		ImNodes::EndCanvas();	

		//if (showGraph)
		//	ShowExampleAppCustomNodeGraph(&graphOpened);
	}

	ImGui::End();
}
