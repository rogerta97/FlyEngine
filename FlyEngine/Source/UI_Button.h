#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "UI_Element.h"
#include "SaveAndLoad.h"
#include <string>
#include <list>

using namespace std;

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
	void SetBackgroundTexture(Texture* buttonTexture);

private:
	// Easy Acces to Holder Action
	DisplayImageAction* uiObjectDisplayImageBackground;
	std::list<Action*> onClickActions; 

	Texture* idleTexture;
};

#endif // !_UI_IMAGE_H_
