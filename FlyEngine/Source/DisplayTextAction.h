#ifndef _DISPLAY_TEXT_ACTION_H_
#define _DISPLAY_TEXT_ACTION_H_

#include "Action.h"
#include "Font.h"
#include "MathGeoLib/Math/float4.h"
#include <string>
#include <map>

class Quad; 
using namespace std;

class BoundingBox; 
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

	// Positioning -------------------------
	void CalculateOriginTextPosition(); 

	// Set & Get ---------------------------
	void SetText(std::string newText);
	std::string& GetText();

	void SetFont(Font* newFont);
	Font* GetFont();

	void SetTextBoxSize(BoundingBox* newFont);
	BoundingBox* GetTextBox();

	void SetTextColor(float4 newColor);
	float4& GetTextColor();

	void UpdateTextQuadsSize(); 

private:	
	void AllocateTextQuads(int amount, int position = -1);

private: 
	Font* textFont = nullptr; 
	std::string text; 
	float4 textColor; 

	// Text Box ----------
	BoundingBox* textBox; 
	float2 originTextPosition; 
	
	int quadsAllocated = 0; 
	std::map<int, Quad*> textQuads;
};

#endif 