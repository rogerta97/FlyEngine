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
{
	graphNodeList.clear();
	connectionsList.clear(); 
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::SelectNode(string nodeToSelect)
{
	for (auto it : instance->graphNodeList) {

		if(it->title != nodeToSelect)
			it->selected = false;
		else
			it->selected = true;
	}
}

void NodeGraph::CreateNode(string nodeName, ImVec2 pos, UID roomID)
{
	Node* newNode = new Node();

	newNode->title = nodeName.c_str();
	newNode->selected = false;
	newNode->position = pos;
	newNode->inputs.push_back({ "In", 1 });
	newNode->outputs.push_back({ "Out", 1 });
	newNode->roomID = roomID; 

	instance->graphNodeList.push_back(newNode);
}

void NodeGraph::DeleteNode(string nodeName)
{
	for (auto it = instance->graphNodeList.begin(); it != instance->graphNodeList.end(); it++) {
		
		if ((*it)->title == nodeName) {
			delete (*it); 
			instance->graphNodeList.erase(it);
			return;
		}
	}
}

std::list<Node*>& NodeGraph::GetNodeList()
{
	return instance->graphNodeList;
}

std::string NodeGraph::GetNodesAsCombo()
{
	std::string returnString;
	for (auto it : instance->graphNodeList) {
		returnString += it->title;
		returnString += '\0'; 
	}

	return returnString; 
}

void NodeGraph::DeleteAllNodes()
{
	for (auto it = instance->graphNodeList.begin(); it != instance->graphNodeList.end(); it++) {
		delete (*it);
	}

	instance->graphNodeList.clear();
	delete instance; 

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

void NodeGraph::SelectConnection(UID connectionID)
{
	for (auto it : instance->connectionsList) {
		if (it->connectionID == connectionID) {
			it->isSelected = true; 
			instance->connectionSelected = it;
		}
		else {
			it->isSelected = false; 
		}
	}
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

void NodeGraph::DeleteConnections(vector<UID> connectionsToDelIDVector)
{
	for (auto currentConnection = instance->connectionsList.begin(); currentConnection != instance->connectionsList.end();) 
	{
		for (auto checkID = connectionsToDelIDVector.begin(); checkID != connectionsToDelIDVector.end();)
		{
			if ((*currentConnection)->connectionID == (*checkID))
			{
				delete (*currentConnection); 
				currentConnection = instance->connectionsList.erase(currentConnection); 
				checkID = connectionsToDelIDVector.erase(checkID);
				break;
			}
			else {

				checkID++; 
				currentConnection++; 
			}
		}
	}	
}

int NodeGraph::DeleteAllConnections()
{
	int conexionsDeletedAmount = instance->connectionsList.size(); 
	for (auto it = instance->connectionsList.begin(); it != instance->connectionsList.end(); it++) {
		delete (*it);
	}

	instance->connectionsList.clear();

	FLY_LOG("Connections Deleted Correctly");
	return conexionsDeletedAmount; 
}

void NodeGraph::CheckNewConnection()
{
	void* _originNode;
	void* _dstNode; 
	const char* _originSlotName; 
	const char* _dstSlotName; 
	
	if (ImNodes::GetNewConnection(&_dstNode, &_dstSlotName, &_originNode, &_originSlotName)) {

		Node* originNode = (Node*)_originNode; 
		Room* originRoom = App->moduleWorldManager->GetRoom(originNode->title); 

		Node* dstNode = (Node*)_dstNode;
		Room* dstRoom = App->moduleWorldManager->GetRoom(dstNode->title);

		originRoom->ConnectToRoom(dstRoom); 
	}
}

void NodeGraph::DrawNodeGraph()
{

	static ImNodes::CanvasState canvas;
	ImNodes::BeginCanvas(&canvas);

	if (instance == nullptr) {
		ImNodes::EndCanvas();
		return;
	}

	instance->DrawNodeConnections();

	for (auto it : instance->graphNodeList) {

		if (ImNodes::Ez::BeginNode(it, it->title.c_str(), &it->position, &it->selected))
		{
			ImGui::Image(0, ImVec2(100, 100)); 
			ImNodes::Ez::InputSlots(it->inputs);
			ImNodes::Ez::OutputSlots(it->outputs);
			ImNodes::Ez::EndNode(it->title.c_str());
		} 
	}

	instance->CheckNewConnection();


	ImNodes::EndCanvas(); 
}

Node* NodeGraph::GetNodeByTitle(string nodeName)
{
	for (list<Node*>::iterator it = graphNodeList.begin(); it != graphNodeList.end(); it++) {
		if ((*it)->title == nodeName)
			return *it; 
	}

	return nullptr; 
}

NodeGraphConnection::NodeGraphConnection()
{
	SetAllNullptr(); 
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
	isSelected = false;
}

void NodeGraphConnection::DrawConnection()
{
	if (ImNodes::Connection(destinationNode, destinationSlotName.c_str(), originNode, originSlotName.c_str(), isSelected))
	{
		NodeGraph::getInstance()->connectionSelected = this; 
	}
}
