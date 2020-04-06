#ifndef _IMNODES_GRAPH_H_
#define _IMNODES_GRAPH_H_

#include "Globals.h"
#include "imnodes.h"

#include <list>
#include <string>

//class DialogueSlot; 
//struct GraphNode
//{
//	GraphNode();
//	~GraphNode(); 
//
//	UID nodeID; 
//};
//
//struct DialogNode : GraphNode
//{
//	DialogNode();
//	~DialogNode();
//
//	DialogueSlot* dialogueSlot; 
//};
//
//class Room; 
//struct RoomNode : GraphNode
//{
//	RoomNode();
//	~RoomNode();
//};

class Dialogue;
class DialogueNodeGraph
{
public:
	DialogueNodeGraph();
	~DialogueNodeGraph(); 

	void DrawGraph(); 

	Dialogue* GetDialogueData();
	void AttachDialogue(Dialogue* newDialogue);

private:
	Dialogue* dialogue;
};

#endif
