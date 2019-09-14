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

	ImNodes::Ez::SlotInfo inputs[1]; 
	ImNodes::Ez::SlotInfo outputs[1];
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
	static void AddConnection(string originNodeTitle, string dstNodeTitle); 

	static void DrawNodeGraph();

private: 
	Node* FindNode(string nodeName);

private:
	list<Node*> nodeList;

};

//class NodeGraph 
//{
//	static NodeGraph* instance;
//	NodeGraph();
//
///*public:
//	static NodeGraph* getInstance() {
//		if (!instance)
//			instance = new NodeGraph;
//
//		return instance;
//	}
//
//public: 
//	~NodeGraph();
//
//	static void CreateNodes(); 
//	static void DeleteNodes(); 
//
//	static void DrawNodeGraph(); 
//
//private: 
//	list<Node*> nodeList;*/ 
//};

#endif