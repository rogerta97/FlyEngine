#ifndef _DISPLAY_TEXT_ACTION_H_
#define _DISPLAY_TEXT_ACTION_H_

#include "Action.h"
#include <string>

using namespace std;

class Font; 
class DisplayTextAction : public Action
{
public:
	DisplayTextAction(FlyObject* _parentObject);
	~DisplayTextAction();

	void SetText(std::string newText);
	std::string& GetText();

private: 
	Font* currentFont = nullptr; 
	std::string text; 

};

#endif 