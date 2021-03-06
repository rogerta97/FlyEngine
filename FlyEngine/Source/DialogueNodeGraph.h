#ifndef _IMNODES_GRAPH_H_
#define _IMNODES_GRAPH_H_

#include "Globals.h"
#include "imnodes.h"

#include <list>
#include <vector>
#include <string>

class Dialogue;
class DialogueStep;
class StepAnswer; 
class DialogueNodeGraph
{
public:
	DialogueNodeGraph();
	~DialogueNodeGraph(); 

	void DrawGraph();
	void HandleRightClickPopups(int nodeHovered);
	void DrawGraphLinks();
	void DrawGraphNodes();
	void HandleNodeClick();
	void DrawAnswerPins(DialogueStep* currentStep);

	std::list<StepAnswer*> GetGraphLinkFromDstUID(UID dstUID);
	void EraseGraphLink(UID start_attr);
	void EraseGraphNode(UID start_attr);

	Dialogue* GetDialogueData();
	void AttachDialogue(Dialogue* newDialogue);

	void GenerateLinksFromDialogueAttached(); 

	bool saveData = false; 

private:

	Dialogue* dialogue;
	std::vector<std::pair<int, int>> links;
};

#endif
