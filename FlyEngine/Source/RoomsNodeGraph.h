#ifndef _ROOMS_NODE_GRAPH_H_
#define _ROOMS_NODE_GRAPH_H_

#include "Globals.h"
#include "imnodes.h"

class ModuleWorldManager; 
class RoomsNodeGraph
{
public:
	RoomsNodeGraph();
	~RoomsNodeGraph();

	void DrawGraph(); 
	void Update(); 

private: 
	ModuleWorldManager* roomManager; 
	
};

#endif
