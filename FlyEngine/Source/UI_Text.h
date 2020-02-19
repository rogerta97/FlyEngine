#ifndef _UI_TEXT_H_
#define _UI_TEXT_H_

#include "UI_Element.h"
#include "SaveAndLoad.h"
#include <string>

using namespace std;

class FlyObject;
class DisplayTextAction; 
class UI_Text : public UI_Element
{
public:

	UI_Text();
	~UI_Text();

	void Init();
	void Update();
	void Draw();
	void CleanUp();

	DisplayTextAction* GetDisplayTextAction();

private:

	// Easy Acces to Holder Action
	DisplayTextAction* displayTextAction;
};

#endif // !_UI_IMAGE_H_
