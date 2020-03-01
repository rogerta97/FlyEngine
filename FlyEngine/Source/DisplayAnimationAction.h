#ifndef _DISPLAY_ANIMATION_ACTION_H_
#define _DISPLAY_ANIMATION_ACTION_H_

#define MAX_ROOMS 256

#include "Action.h"
#include <list>

using namespace std;

class Texture;
class Animation; 
class DisplayAnimationAction : public Action
{
public:
	DisplayAnimationAction(FlyObject* _parentObject);
	~DisplayAnimationAction();

	void Init();
	void Update();
	void CleanUp();

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

	void DrawUISettings();
	void DrawSettingsRightColumn();
	void DrawUISettingsLeftColumn(float squareSize);
	void DrawAddFramePopup(); 

	// Set & Get ---
	Animation* GetAnimation(); 

	Texture* GetCurrentFrame();
	void SetCurrentFrame(Texture* currentFrame); 

private:
	Animation* animation; 
	Texture* currentFrame; 
};

#endif 