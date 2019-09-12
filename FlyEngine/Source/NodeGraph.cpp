#include "NodeGraph.h"
#include "ImNode.h"
#include "ImNodeEz.h"

#include "Application.h"
#include "Room.h"
#include "ModuleWorldManager.h"
#include "mmgr.h"

NodeGraph* NodeGraph::instance = 0;

NodeGraph* NodeGraph::getInstance()
{
	if (instance == 0) {
		instance = new NodeGraph();
	}

	return instance;
}

NodeGraph::NodeGraph()
{}

NodeGraph::~NodeGraph()
{

}

void NodeGraph::CreateNodes()
{
	for (auto it = App->moduleWorldManager->roomsInWorldList.begin(); it != App->moduleWorldManager->roomsInWorldList.end(); it++){

		Node* newNode = new Node();

		newNode->title = (*it)->GetName();
		newNode->selected = false; 
		newNode->position = ImVec2(50, 50); 

		instance->nodeList.push_back(newNode); 
	}

	FLY_LOG("Nodes Created Correctly");
}

void NodeGraph::DeleteNodes()
{
	for (auto it = instance->nodeList.begin(); it != instance->nodeList.end(); it++) {
		delete (*it);
	}

	instance->nodeList.clear();
	delete instance; 

	FLY_LOG("Nodes Deleted Correctly");
}

void NodeGraph::DrawNodeGraph()
{
	if (instance == nullptr)
		return; 

	static ImNodes::CanvasState canvas;
	ImNodes::BeginCanvas(&canvas);

	for (auto it = instance->nodeList.begin(); it != instance->nodeList.end(); it++) {

		if (ImNodes::Ez::BeginNode((*it), (*it)->title.c_str(), &(*it)->position, &(*it)->selected))
		{
			//ImNodes::Ez::InputSlots(node.inputs, 1);
			//ImNodes::Ez::OutputSlots(node.outputs, 1);
			ImNodes::Ez::EndNode();
		}

	}

	ImNodes::EndCanvas(); 
}
