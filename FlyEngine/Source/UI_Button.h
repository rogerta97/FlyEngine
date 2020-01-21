#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "UI_Element.h"
#include "SaveAndLoad.h"
#include <string>
#include <list>

using namespace std;

enum ButtonBehaviourMouseInteraction
{
	COLOR_TINT,
	TEXTURE_SWAP,
	INTERACTION_NONE,
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
	void SetMainTexture(Texture* buttonTexture);
	DisplayImageAction* GetDisplayImageAction();

	// Get Textures 
	Texture* GetMainTexture(); 
	Texture* GetIdleTexture(); 
	Texture* GetMouseOverTexture(); 
	Texture* GetMouseClickedTexture(); 

public:
	ButtonBehaviourMouseInteraction behaviourInteraction; 

private:
	// Easy Acces to Holder Action
	DisplayImageAction* uiObjectDisplayImageBackground;
	std::list<Action*> onClickActions; 

	// Color Tint Mode
	Texture* backgroundTexture; 

	// Texture Swap Mode 
	Texture* idleTexture;
	Texture* mouseOverTexture;
	Texture* mouseClickedTexture;
};

#endif // !_UI_IMAGE_H_
