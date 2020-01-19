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
class UI_Button : public UI_Element
{
public:
	UI_Button();
	~UI_Button();

	void Update();
	void Draw();
	void CleanUp();

	void Save(JSON_Object* jsonObject, string serializeStr);
	void Load(JSON_Object* jsonObject, string serializeStr);

private:

	// Easy Acces to Holder Action
	DisplayImageAction* uiObjectDisplayImage;
	std::list<Action*> onClickActions; 

};

#endif // !_UI_IMAGE_H_
