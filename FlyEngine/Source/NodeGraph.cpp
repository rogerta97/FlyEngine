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
{}

void NodeGraph::SelectNode(string nodeToSelect)
{
	for (auto it : instance->nodeList) {

		if(it->title != nodeToSelect)
			it->selected = false;
		else
			it->selected = true;
	}
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

std::list<Node*> NodeGraph::GetNodeList()
{
	return instance->nodeList;
}

std::string NodeGraph::GetNodesAsCombo()
{
	std::string returnString;
	for (auto it : instance->nodeList) {
		returnString += it->title;
		returnString += '\0'; 
	}

	return returnString; 
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

list<NodeGraphConnection*> NodeGraph::GetConnectionList()
{
	return instance->connectionsList;
}

std::string NodeGraph::GetConnectionsAsCombo()
{
	std::string returnString;
	for (auto it : instance->connectionsList) {
		returnString += "Should Add Sth";
		returnString += '\0';
	}

	return returnString;
}

void NodeGraph::ConnectNodes(string originNodeTitle, string originSlotName, string destinationNodeTitle, string destinationSlotName, UID logicConnectionID)
{
	Node* originNode = instance->GetNodeByTitle(originNodeTitle); 
	Node* destinationNode = instance->GetNodeByTitle(destinationNodeTitle);

	NodeGraphConnection* newGraphConnection = new NodeGraphConnection(); 
	newGraphConnection->connectionID = logicConnectionID; 
	newGraphConnection->SetConnectionOrigin(originNode, originSlotName); 
	newGraphConnection->SetConnectionDestination(destinationNode, destinationSlotName); 

	instance->connectionsList.push_back(newGraphConnection); 

	FLY_LOG("NEW GRAPH CONNECTION: %s -> %s", originNodeTitle.c_str(), destinationNodeTitle.c_str());
	FLY_LOG("NEW GRAPH CONNECTION ID: %f", logicConnectionID); 
}

void NodeGraph::ConnectNodes(Node* originNode, string originSlotName, Node* destinationNode, string destinationSlotName, UID logicConnectionID)
{
	NodeGraphConnection* newGraphConnection = new NodeGraphConnection();
	newGraphConnection->connectionID = logicConnectionID;
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
	void* _originNode;
	void* _dstNode; 
	const char* _originSlotName; 
	const char* _dstSlotName; 
	
	if (ImNodes::GetNewConnection(&_dstNode, &_dstSlotName, &_originNode, &_originSlotName)) {

		Node* originNode = (Node*)_originNode; 
		Room* originRoom = App->moduleWorldManager->GetRoomByName(originNode->title); 

		Node* dstNode = (Node*)_dstNode;
		Room* dstRoom = App->moduleWorldManager->GetRoomByName(dstNode->title);

		App->moduleWorldManager->ConnectRooms(originRoom, dstRoom); 
	}

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
			ImGui::Image(0, ImVec2(100, 100)); 
			ImNodes::Ez::InputSlots(it->inputs);
			ImNodes::Ez::OutputSlots(it->outputs);
			ImNodes::Ez::EndNode(it->title.c_str());
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
