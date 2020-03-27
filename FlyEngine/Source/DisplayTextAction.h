#ifndef _DISPLAY_TEXT_ACTION_H_
#define _DISPLAY_TEXT_ACTION_H_

#include "Action.h"
#include "Font.h"
#include "MathGeoLib/Math/float4.h"
#include <string>
#include <map>
#include <vector>

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
	void Update(float dt);
	void CleanUp();
	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);
	void DrawUISettings();
	void DrawUISettingsInButton();

	// Render ------------------------------
	void RenderText(); 
	void DrawTextBox(); 

	// Utility -----------------------------
	void CleanQuads();

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

	void SetDrawTextBox(bool _draw);
	bool& GetDrawTextBox();

	void SetLineSpacing(float _lineSpacing);
	float& GetLineSpacing();

	void UpdateTextQuads(); 

private: 
	Font* textFont = nullptr; 
	std::string text; 
	float4 textColor; 
	float lineSpacing = 0.0f; 

	// Text Box ----------
	BoundingBox* textBox; 
	float2 originTextPosition; 
	bool drawTextBox; 
	float2 pevBoxPos; 
	
	int quadsAllocated = 0; 
	std::vector<Quad*>* textQuads;
};

#endif 