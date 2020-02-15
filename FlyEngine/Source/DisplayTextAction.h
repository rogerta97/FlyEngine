#ifndef _DISPLAY_TEXT_ACTION_H_
#define _DISPLAY_TEXT_ACTION_H_

#include "Action.h"

using namespace std;

class Font; 
class DisplayTextAction : public Action
{
public:
	DisplayTextAction(FlyObject* _parentObject);
	~DisplayTextAction();

private: 
	Font* currentFont = nullptr; 
	std::string text; 

};

#endif 