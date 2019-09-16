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

void NodeGraph::Update()
{
	instance->DrawNodeGraph();

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
	newNode->inputs.push_back({ "In", 1 });
	newNode->outputs.push_back({ "Out", 1 });

	instance->nodeList.push_back(newNode);

	FLY_LOG("Node Created");
}

void NodeGraph::DeleteNode(string nodeName)
{
	for (auto it = instance->nodeList.begin(); it != instance->nodeList.end(); it++) {
		
		if ((*it)->title == nodeName) {
			delete (*it); 
			instance->nodeList.erase(it);
			return;
		}
	}
}

void NodeGraph::DeleteAllNodes()
{
	for (auto it = instance->nodeList.begin(); it != instance->nodeList.end(); it++) {
		delete (*it);
	}

	instance->nodeList.clear();

	FLY_LOG("Nodes Deleted Correctly");
}

void NodeGraph::DrawNodeConnections()
{
	for (auto it : instance->connectionsList) {
		it->DrawConnection();
	}	
}

void NodeGraph::ConnectNodes(string originNodeTitle, string originSlotName, string destinationNodeTitle, string destinationSlotName)
{
	Node* originNode = instance->GetNodeByTitle(originNodeTitle); 
	Node* destinationNode = instance->GetNodeByTitle(destinationNodeTitle);

	NodeGraphConnection* newGraphConnection = new NodeGraphConnection(); 

	newGraphConnection->SetConnectionOrigin(originNode, originSlotName); 
	newGraphConnection->SetConnectionDestination(destinationNode, destinationSlotName); 

	instance->connectionsList.push_back(newGraphConnection); 
}

void NodeGraph::ConnectNodes(Node* originNode, string originSlotName, Node* destinationNode, string destinationSlotName)
{
	NodeGraphConnection* newGraphConnection = new NodeGraphConnection();

	newGraphConnection->SetConnectionOrigin(originNode, originSlotName);
	newGraphConnection->SetConnectionDestination(destinationNode, destinationSlotName);

	instance->connectionsList.push_back(newGraphConnection);
}

void NodeGraph::DeleteConnection(int connectionID)
{
	for (auto it = instance->connectionsList.begin(); it != instance->connectionsList.end(); it++) {

		if ((*it)->connectionID == connectionID) {
			delete (*it);
			instance->connectionsList.erase(it);
			return;
		}
	}
}

void NodeGraph::DeleteAllConnections()
{
	for (auto it = instance->connectionsList.begin(); it != instance->connectionsList.end(); it++) {
		delete (*it);
	}

	instance->connectionsList.clear();
	delete instance;

	FLY_LOG("Connections Deleted Correctly");
}

void NodeGraph::CheckNewConnection()
{
	void* originNode;
	void* dstNode; 
	const char* originSlotName; 
	const char* dstSlotName; 
	
	if (ImNodes::GetNewConnection(&dstNode, &dstSlotName, &originNode, &originSlotName))	
		instance->ConnectNodes((Node*)originNode, originSlotName, (Node*)dstNode, dstSlotName); 
	
}

void NodeGraph::DrawNodeGraph()
{
	if (instance == nullptr)
		return; 

	static ImNodes::CanvasState canvas;
	ImNodes::BeginCanvas(&canvas);

	for (auto it : instance->nodeList) {

		if (ImNodes::Ez::BeginNode(it, it->title.c_str(), &it->position, &it->selected))
		{
			ImNodes::Ez::InputSlots(it->inputs);
			ImNodes::Ez::OutputSlots(it->outputs);
			ImNodes::Ez::EndNode();
		} 
	}

	instance->CheckNewConnection();
	instance->DrawNodeConnections(); 

	ImNodes::EndCanvas(); 
}

Node* NodeGraph::GetNodeByTitle(string nodeName)
{
	for (list<Node*>::iterator it = nodeList.begin(); it != nodeList.end(); it++) {
		if ((*it)->title == nodeName)
			return *it; 
	}

	return nullptr; 
}

NodeGraphConnection::NodeGraphConnection()
{

}

void NodeGraphConnection::SetConnectionOrigin(Node* originNode, string slotTitle)
{
	this->originNode = originNode;
	this->originSlotName = slotTitle; 
}

void NodeGraphConnection::SetConnectionDestination(Node* destinationNode, string slotTitle)
{
	this->destinationNode = destinationNode;
	this->destinationSlotName = slotTitle;
}

void NodeGraphConnection::SetAllNullptr()
{
	destinationNode = nullptr; 
	originNode = nullptr;
	destinationSlotName = ""; 
	originSlotName = "";
	connectionID = -1; 
	isBidirecitonal = false; 
}

void NodeGraphConnection::DrawConnection()
{
	ImNodes::Connection(destinationNode, destinationSlotName.c_str(), originNode, originSlotName.c_str()); 
}
