#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "UI_Element.h"
#include "SaveAndLoad.h"
#include "Math/float4.h"
#include <string>
#include <list>

using namespace std;

enum ButtonBehaviourMouseInteraction
{
	COLOR_TINT,
	TEXTURE_SWAP,
	INTERACTION_NONE,
};

enum ButtonTextureType
{
	MAIN_TEXTURE,
	MOUSE_OVER_TEXTURE,
	MOUSE_CLICKED_TEXTURE, 
};

class FlyObject;
class DisplayImageAction;
class Action; 
class Texture; 
class UI_Button : public UI_Element
{
public:
	UI_Button();
	~UI_Button();

	void Init(); 
	void Update();
	void Draw();
	void CleanUp();

	void Save(JSON_Object* jsonObject, string serializeStr);
	void Load(JSON_Object* jsonObject, string serializeStr);

	// Utility ---------
	DisplayImageAction* GetDisplayImageAction();
	void AddOnClickAction(Action* newAction);
	std::list<Action*>& GetOnClickActionList(); 

	// Set Color Tints 
	void SetMouseOverTintColor(float4 newColor);
	void SetMouseClickedTintColor(float4 newColor);

	// Set Textures
	void SetMainTexture(Texture* buttonTexture);
	void SetMouseOverTexture(Texture* buttonTexture);
	void SetMouseClickedTexture(Texture* buttonTexture);

	// Get Textures 
	Texture* GetMainTexture(); 
	Texture* GetMouseOverTexture(); 
	Texture* GetMouseClickedTexture(); 

public:
	ButtonBehaviourMouseInteraction mouseInteraction; 
	ButtonTextureType textureTypeShowing; 

private:
	// Easy Acces to Holder Action
	DisplayImageAction* uiObjectDisplayImageBackground;
	std::list<Action*> onClickActions; 

	// Color Tint Mode
	Texture* backgroundTexture; 
	float4 mouseOverTint; 
	float4 mouseClickedTint; 

	// Texture Swap Mode 
	Texture* mainTexture;
	Texture* mouseOverTexture;
	Texture* mouseClickedTexture;
};

#endif // !_UI_IMAGE_H_
