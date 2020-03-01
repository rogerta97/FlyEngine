#ifndef _DISPLAY_ANIMATION_ACTION_H_
#define _DISPLAY_ANIMATION_ACTION_H_

#define MAX_ROOMS 256

#include "Action.h"
#include <list>

using namespace std;

enum AnimationState
{
	ANIMATION_PLAY,
	ANIMATION_PAUSE, 
};

class Texture;
class Animation; 
class DisplayAnimationAction : public Action
{
public:
	DisplayAnimationAction(FlyObject* _parentObject);
	~DisplayAnimationAction();

	void Init();
	void Update(float dt);
	void CleanUp();

	void Play(); 
	void NextFrame(); 

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false);

	void DrawUISettings();
	void DrawSettingsRightColumn();
	void DrawUISettingsLeftColumn(float squareSize);
	void DrawAddFramePopup(); 

	// Set & Get ---
	Animation* GetAnimation(); 

	int GetCurrentFrame();
	void SetCurrentFrame(int currentFrame); 

	AnimationState animationState = ANIMATION_PAUSE;

private:

	Animation* animation; 
	int currentFrame; 

	// Play Animation
	float animationTime; 
};

#endif 