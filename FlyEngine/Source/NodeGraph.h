#ifndef _NODE_GRAPH_H_
#define _NODE_GRAPH_H_

#include "imgui.h"
#include "ImNode.h"
#include "ImNodeEz.h"
#include "Globals.h"

#include <list>
#include <vector>
#include <string>

using namespace std;
using namespace ImNodes; 
using namespace Ez; 

enum NodeGraphType
{
	ROOM_GRAPH,
	DIALOGUE_GRAPH
};

struct Node {

	UID nodeID; 

	ImVec2 position;
	bool selected; 
	string title; 

	list<SlotInfo> inputs; 
	list<SlotInfo> outputs;
};

struct DialogueNode : public Node
{
	string sentence; 
	list<string> answersStrList; 
};

struct RoomNode : public Node
{
	string roomTitle;
	ImTextureID sceneTextureID;
};

struct NodeGraphConnection {

	NodeGraphConnection();

public:
	void SetConnectionOrigin(Node* originNode, string slotTitle);
	void SetConnectionDestination(Node* deatinationNode, string slotTitle);

	void SetAllNullptr(); 

	void DrawConnection();

public:
	bool isSelected; 
	bool isBidirecitonal;
	int connectionID; 

	Node* destinationNode; 
	Node* originNode; 
	
	string destinationSlotName; 
	string originSlotName;
};

class NodeGraph
{
public:
	NodeGraph(NodeGraphType nodeGraphType);
	~NodeGraph(); 

	void Update();
	void DrawNodeGraph();

	// Nodes
	void SelectNode(string nodeToSelect); 
	void CreateNode(string nodeName, ImVec2 pos, UID nodeUID);
	void DeleteNode(string nodeName);

	list<Node*>& GetNodeList();
	std::string GetNodesAsCombo(); 
	Node* GetNode(UID nodeID);

	void DeleteAllNodes();

	// Connections
	void ConnectNodes(string originNodeTitle, string originSlotName, string destinationNodeTitle, string destinationSlotName, UID logicConectionID);
	void ConnectNodes(Node* originNode, string originSlotName, Node* destinationNode, string destinationSlotName, UID logicConectionID);
	void DrawNodeConnections(); 

	list<NodeGraphConnection*> GetConnectionList();
	std::string GetConnectionsAsCombo();
	void SelectConnection(UID connectionID); 

	void DeleteConnection(int connectionID); 
	void DeleteConnections(vector<UID> connectionsList); 
	int DeleteAllConnections(); 

	void CheckNewConnection(); 

	NodeGraphConnection* connectionSelected = nullptr;
	NodeGraphType nodeGraphType; 

private: 
	Node* GetNodeByTitle(string nodeName);

private:
	ImNodes::CanvasState canvas;

	list<Node*> graphNodeList;
	list<NodeGraphConnection*> connectionsList; 

 

};

#endif