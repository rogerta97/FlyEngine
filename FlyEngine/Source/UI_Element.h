#ifndef _UI_ELEMENT_H_
#define _UI_ELEMENT_H_

#include "Globals.h"
#include "SaveAndLoad.h"
#include <string>

enum UIElementType
{
	UI_IMAGE,
	UI_BUTTON,
	UI_TEXT,
	UI_null,
};

class FlyObject; 
class Transform;
class UI_Element
{
public:
	UI_Element();
	~UI_Element();

	virtual void Init(); 
	virtual void Update(); 
	virtual void Draw(); 
	virtual void CleanUp(); 
	
	virtual void Save(JSON_Object* jsonObject, std::string serializeStr);
	virtual void Load(JSON_Object* jsonObject, std::string serializeStr);
	void LoadTransform(JSON_Object* jsonObject, std::string serializeStr, Transform* objectTransform);

	FlyObject* GetHolderObject(); 
	FlyObject* CreateBaseUI(); 
	UID GetUID(); 

public: 
	bool isSelected; 
	UIElementType uiElementType; 
	bool showValueConditions; 

protected: 
	FlyObject* uiObject; 
	UID uid; 
};

#endif // !_UI_IMAGE_H_
