#ifndef _NODE_GRAPH_H_
#define _NODE_GRAPH_H_

#include "imgui.h"
#include "ImNode.h"
#include "ImNodeEz.h"

#include <list>
#include <string>

using namespace std;

struct Node {

	ImVec2 position;
	bool selected; 
	string title; 

	list<ImNodes::Ez::SlotInfo> inputs; 
	list<ImNodes::Ez::SlotInfo> outputs;
};

class NodeGraph
{
private:
	static NodeGraph* instance;
	NodeGraph();

public:
	static NodeGraph* getInstance();
	~NodeGraph();

	static void CreateNode(string nodeName, ImVec2 pos);
	static void DeleteNodes();
	static void DrawNodeConnections(); 

	static void DrawNodeGraph();

private: 
	Node* FindNode(string nodeName);

private:
	list<Node*> nodeList;

};

#endif