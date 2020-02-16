#ifndef _DISPLAY_TEXT_ACTION_H_
#define _DISPLAY_TEXT_ACTION_H_

#include "Action.h"
#include <string>
#include <map>

class Quad; 
using namespace std;

class Font; 
class DisplayTextAction : public Action
{
public:
	DisplayTextAction(FlyObject* _parentObject);
	~DisplayTextAction();

	// Virtual -----------------------------
	void Draw();
	void CleanUp();
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);
	void DrawUISettings();

	// Render ------------------------------
	void RenderText(); 

	// Set & Get ---------------------------
	void SetText(std::string newText);
	std::string& GetText();

	void SetFont(Font* newFont);
	Font* GetFont();

private:	
	void AllocateTextQuads(int amount, int position = -1);

private: 
	Font* textFont = nullptr; 
	std::string text; 
	
	int quadsAllocated = 0; 
	std::map<int, Quad*> textQuads;
};

#endif 