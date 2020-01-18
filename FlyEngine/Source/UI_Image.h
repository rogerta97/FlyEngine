#ifndef _UI_IMAGE_H_
#define _UI_IMAGE_H_

#include "UI_Element.h"
#include <string>

using namespace std; 

class FlyObject;
class DisplayImageAction; 
class UI_Image : public UI_Element
{
public: 
	UI_Image(); 
	~UI_Image();

	void Draw(); 
	void CleanUp(); 

	void Create(string imagePath); 

private: 
	DisplayImageAction* uiObjectDisplayImage;
};

#endif // !_UI_IMAGE_H_
