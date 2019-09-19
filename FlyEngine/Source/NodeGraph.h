#ifndef _NODE_GRAPH_H_
#define _NODE_GRAPH_H_

#include "imgui.h"
#include "ImNode.h"
#include "ImNodeEz.h"
#include "Globals.h"

#include <list>
#include <string>

using namespace std;
using namespace ImNodes; 
using namespace Ez; 

struct Node {

	ImVec2 position;
	bool selected; 
	string title; 

	list<SlotInfo> inputs; 
	list<SlotInfo> outputs;
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
private:
	static NodeGraph* instance;
	NodeGraph();

public:
	static NodeGraph* getInstance();
	~NodeGraph();

	static void Update();
	static void DrawNodeGraph();

	// Nodes
	static void SelectNode(string nodeToSelect); 
	static void CreateNode(string nodeName, ImVec2 pos);
	static void DeleteNode(string nodeName);

	static list<Node*> GetNodeList();
	static std::string GetNodesAsCombo(); 

	static void DeleteAllNodes();

	// Connections
	static void ConnectNodes(string originNodeTitle, string originSlotName, string destinationNodeTitle, string destinationSlotName, UID logicConectionID);
	static void ConnectNodes(Node* originNode, string originSlotName, Node* destinationNode, string destinationSlotName, UID logicConectionID);
	static void DrawNodeConnections(); 

	static list<NodeGraphConnection*> GetConnectionList();
	static std::string GetConnectionsAsCombo();
	static void SelectConnection(UID connectionID); 

	static void DeleteConnection(int connectionID); 
	static void DeleteAllConnections(); 

	static void CheckNewConnection(); 

	NodeGraphConnection* connectionSelected = nullptr;

private: 
	Node* GetNodeByTitle(string nodeName);

private:
	list<Node*> graphNodeList;
	list<NodeGraphConnection*> connectionsList; 

 

};

#endif