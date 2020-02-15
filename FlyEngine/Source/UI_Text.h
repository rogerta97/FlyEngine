#ifndef _UI_TEXT_H_
#define _UI_TEXT_H_

#include "UI_Element.h"
#include "SaveAndLoad.h"
#include <string>

using namespace std;

class FlyObject;
class DisplayImageAction;
class UI_Image : public UI_Element
{
public:
	UI_Image();
	~UI_Image();

	void Update();
	void Draw();
	void CleanUp();

	void Save(JSON_Object* jsonObject, string serializeStr);
	void Load(JSON_Object* jsonObject, string serializeStr);

	void Create(string imagePath);
	DisplayImageAction* GetDisplayImage();

private:
	// Easy Acces to Holder Action
	DisplayImageAction* uiObjectDisplayImage;
};

#endif // !_UI_IMAGE_H_
