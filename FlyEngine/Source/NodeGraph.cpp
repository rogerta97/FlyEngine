#include "NodeGraph.h"
#include "ImNode.h"
#include "ImNodeEz.h"
#include "ModuleWorldManager.h"

NodeGraph::NodeGraph()
{
}

NodeGraph::~NodeGraph()
{
}

void NodeGraph::DrawNodeGraph()
{
	static ImNodes::CanvasState canvas;
	ImNodes::BeginCanvas(&canvas);

	ImNodes::EndCanvas(); 
}
