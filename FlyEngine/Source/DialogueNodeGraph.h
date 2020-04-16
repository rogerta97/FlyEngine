#ifndef _IMNODES_GRAPH_H_
#define _IMNODES_GRAPH_H_

#include "Globals.h"
#include "imnodes.h"

#include <list>
#include <vector>
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
class DialogueStep;
class DialogueNodeGraph
{
public:
	DialogueNodeGraph();
	~DialogueNodeGraph(); 

	void DrawGraph();
	void DrawGraphLinks();
	void DrawGraphNodes();
	void HandleNodeClick();
	void DrawAnswerPins(DialogueStep* currentStep);


	Dialogue* GetDialogueData();
	void AttachDialogue(Dialogue* newDialogue);

	void GenerateLinksFromDialogueAttached(); 

	bool saveData = false; 

private:
	Dialogue* dialogue;
	std::vector<std::pair<int, int>> links;
};

#endif
