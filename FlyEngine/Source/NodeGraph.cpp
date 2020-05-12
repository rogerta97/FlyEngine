#include "NodeGraph.h"
#include "ImNode.h"
#include "ImNodeEz.h"
#include "RandomNumberGenerator.h"

#include "Application.h"
#include "Room.h"
#include "ModuleWorldManager.h"
#include "mmgr.h"


void NodeGraph::Update()
{
	DrawNodeGraph();
}

NodeGraph::NodeGraph(NodeGraphType _nodeGraphType)
{
	nodeGraphType = _nodeGraphType; 
	graphNodeList.clear();
	connectionsList.clear(); 
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::SelectNode(string nodeToSelect)
{
	for (auto it : graphNodeList) {

		if(it->title != nodeToSelect)
			it->selected = false;
		else
			it->selected = true;
	}
}

void NodeGraph::CreateNode(string nodeName, ImVec2 pos, UID _nodeID)
{
	Node* newNode = new Node();

	newNode->title = nodeName.c_str();
	newNode->selected = false;
	newNode->position = pos;
	newNode->inputs.push_back({ "In", 1 });
	newNode->outputs.push_back({ "Out", 1 });
	newNode->nodeID = _nodeID;

	graphNodeList.push_back(newNode);
}

void NodeGraph::DeleteNode(string nodeName)
{
	for (auto it = graphNodeList.begin(); it != graphNodeList.end(); it++) {
		
		if ((*it)->title == nodeName) {
			delete (*it); 
			graphNodeList.erase(it);
			return;
		}
	}
}

std::list<Node*>& NodeGraph::GetNodeList()
{
	return graphNodeList;
}

std::string NodeGraph::GetNodesAsCombo()
{
	std::string returnString;
	for (auto it : graphNodeList) {
		returnString += it->title;
		returnString += '\0'; 
	}

	return returnString; 
}

Node* NodeGraph::GetNode(UID nodeID)
{
	for (auto& it : graphNodeList) 
	{
		if (it->nodeID == nodeID)
		{
			return it; 
		}
	}

	return nullptr; 
}

void NodeGraph::DeleteAllNodes()
{
	if (graphNodeList.empty())
		return; 

	for (auto it = graphNodeList.begin(); it != graphNodeList.end(); it++) {
		delete (*it);
	}

	graphNodeList.clear();
}

void NodeGraph::DrawNodeConnections()
{
	for (auto it : connectionsList) {
		it->DrawConnection();
	}	
}

list<NodeGraphConnection*> NodeGraph::GetConnectionList()
{
	return connectionsList;
}

std::string NodeGraph::GetConnectionsAsCombo()
{
	std::string returnString;
	for (auto it : connectionsList) {
		returnString += "Should Add Sth";
		returnString += '\0';
	}

	return returnString;
}

void NodeGraph::SelectConnection(UID connectionID)
{
	for (auto it : connectionsList) {
		if (it->connectionID == connectionID) {
			it->isSelected = true; 
			connectionSelected = it;
		}
		else {
			it->isSelected = false; 
		}
	}
}

void NodeGraph::ConnectNodes(string originNodeTitle, string originSlotName, string destinationNodeTitle, string destinationSlotName, UID logicConnectionID)
{
	Node* originNode = GetNodeByTitle(originNodeTitle); 
	Node* destinationNode = GetNodeByTitle(destinationNodeTitle);

	NodeGraphConnection* newGraphConnection = new NodeGraphConnection(); 
	newGraphConnection->connectionID = logicConnectionID; 
	newGraphConnection->SetConnectionOrigin(originNode, originSlotName); 
	newGraphConnection->SetConnectionDestination(destinationNode, destinationSlotName); 

	connectionsList.push_back(newGraphConnection); 

	flog("NEW GRAPH CONNECTION: %s -> %s", originNodeTitle.c_str(), destinationNodeTitle.c_str());
	flog("NEW GRAPH CONNECTION ID: %f", logicConnectionID); 
}

void NodeGraph::ConnectNodes(Node* originNode, string originSlotName, Node* destinationNode, string destinationSlotName, UID logicConnectionID)
{
	NodeGraphConnection* newGraphConnection = new NodeGraphConnection();
	newGraphConnection->connectionID = logicConnectionID;
	newGraphConnection->SetConnectionOrigin(originNode, originSlotName);
	newGraphConnection->SetConnectionDestination(destinationNode, destinationSlotName);

	connectionsList.push_back(newGraphConnection);
}

void NodeGraph::DeleteConnection(int connectionID)
{
	for (auto it = connectionsList.begin(); it != connectionsList.end(); it++) {

		if ((*it)->connectionID == connectionID) {
			delete (*it);
			connectionsList.erase(it);
			return;
		}
	}
}

void NodeGraph::DeleteConnections(vector<UID> connectionsToDelIDVector)
{
	for (auto currentConnection = connectionsList.begin(); currentConnection != connectionsList.end();) 
	{
		for (auto checkID = connectionsToDelIDVector.begin(); checkID != connectionsToDelIDVector.end();)
		{
			if ((*currentConnection)->connectionID == (*checkID))
			{
				delete (*currentConnection); 
				currentConnection = connectionsList.erase(currentConnection); 
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
	int conexionsDeletedAmount = connectionsList.size(); 

	if (conexionsDeletedAmount == 0)
		return false; 

	for (auto it = connectionsList.begin(); it != connectionsList.end(); it++) {
		delete (*it);
	}

	connectionsList.clear();

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
		Room* originRoom = App->moduleRoomManager->GetRoom(originNode->title); 

		Node* dstNode = (Node*)_dstNode;
		Room* dstRoom = App->moduleRoomManager->GetRoom(dstNode->title);

		originRoom->AddConnection(dstRoom); 
	}
}

void NodeGraph::DrawNodeGraph()
{	
	ImNodes::BeginCanvas(&canvas);

	if (nodeGraphType == DIALOGUE_GRAPH)
	{
		for (auto it : graphNodeList) {

			if (ImNodes::Ez::BeginNode(it, it->title.c_str(), &it->position, &it->selected))
			{
				ImNodes::Ez::InputSlots(it->inputs);

				/*for (auto& currentAnswer : it->outputs)
				{

				}*/

				ImNodes::Ez::OutputSlots(it->outputs);
				ImNodes::Ez::EndNode(it->title.c_str());
			}
		}
	}
	if (nodeGraphType == ROOM_GRAPH)
	{
		DrawNodeConnections();

		for (auto it : graphNodeList) {

			if (ImNodes::Ez::BeginNode(it, it->title.c_str(), &it->position, &it->selected))
			{
				ImGui::Image(0, ImVec2(100, 100)); 
				ImNodes::Ez::InputSlots(it->inputs);
				ImNodes::Ez::OutputSlots(it->outputs);
				ImNodes::Ez::EndNode(it->title.c_str());
			} 
		}

		CheckNewConnection();
	}

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
		//NodeGraph::getInstance()->connectionSelected = this; 
	}
}
