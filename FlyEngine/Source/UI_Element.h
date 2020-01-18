#ifndef _UI_ELEMENT_H_
#define _UI_ELEMENT_H_

#include "Globals.h"

class FlyObject; 
class UI_Element
{
public:
	UI_Element();
	~UI_Element();

	virtual void Draw(); 

protected: 
	FlyObject* uiObject; 
	UID uid; 
};

#endif // !_UI_IMAGE_H_
