#ifndef _CONSOLEDOCKPANEL_H_
#define _CONSOLEDOCKPANEL_H_

#define MAX_COMMAND_LENGHT 32

#include "Globals.h"
#include "DockPanel.h"
#include "imgui.h"

#include <list>

using namespace std; 

struct ConsoleMessage {

	string message; 
	consoleMessageType messageType; 
	bool canPrint; 
};

class ConsoleDockPanel : public DockPanel {

public: 

	ConsoleDockPanel(bool isVisible);
	~ConsoleDockPanel(); 

	bool Draw();
	bool CleanUp();
	void PrintMessageStack(); 

	// Utility 
	void SetMessageVisibility(consoleMessageType messageType);
	void AddLineToConsole(consoleMessageType messageType, string textToAdd); 

private:

	ImVec4 debugMessageColor;
	ImVec4 warningMessageColor;
	ImVec4 errorMessageColor;

	int debugMessageAmount;
	int warningMessageAmount;
	int errorMessageAmount; 

	list<ConsoleMessage*> messagesList;
	char commandBuffer[MAX_COMMAND_LENGHT];
};

#endif 
