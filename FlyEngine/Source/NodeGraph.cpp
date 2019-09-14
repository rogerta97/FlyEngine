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

void NodeGraph::CreateNode(string nodeName, ImVec2 pos)
{
	Node* newNode = new Node();

	newNode->title = nodeName.c_str();
	newNode->selected = false;
	newNode->position = pos;
	newNode->inputs[0] = { "In", 1 };
	newNode->outputs[0] = { "Out", 1 };

	instance->nodeList.push_back(newNode);

	FLY_LOG("Node Created");
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

void NodeGraph::AddConnection(string originNodeTitle, string dstNodeTitle)
{

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
			ImNodes::Ez::InputSlots((*it)->inputs, 1);
			ImNodes::Ez::OutputSlots((*it)->outputs, 1);
			ImNodes::Ez::EndNode();
		} 

	}

	ImNodes::EndCanvas(); 
}

Node* NodeGraph::FindNode(string nodeName)
{
	for (list<Node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++) {
		if ((*it)->title == nodeName)
			return *it; 
	}

	return nullptr; 
}
