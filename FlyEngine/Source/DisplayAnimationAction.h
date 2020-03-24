#ifndef _DISPLAY_ANIMATION_ACTION_H_
#define _DISPLAY_ANIMATION_ACTION_H_

#define MAX_ROOMS 256

#include "Action.h"
#include <list>

using namespace std;

enum AnimationState
{
	ANIMATION_PLAY,
	ANIMATION_STOP, 
};

enum AnimationPlayMode
{
	ANIMATION_LOOP,
	ANIMATION_ONE_TIME,
};

class Texture;
class Animation; 
class DisplayImageAction; 
class DisplayAnimationAction : public Action
{
public:
	DisplayAnimationAction(FlyObject* _parentObject);
	~DisplayAnimationAction();

	void CopyData(DisplayAnimationAction* otherAction);
	void AttachToImage(DisplayImageAction* imageToAttach);

	void Init();
	void Update(float dt);
	void CleanUp();
	void DoAction(); 

	void Play(); 
	void Stop(); 
	bool NextFrame(); 
	void AddFrame(Texture* newFrame); 

	void SaveAction(JSON_Object* jsonObject, string serializeObjectString, bool literalStr = false, int actionPositionInObject = 0);

	void DrawUISettings();
	void DrawSettingsRightColumn();
	void DrawUISettingsLeftColumn(float squareSize);
	void DrawAddFramePopup(); 
	void DrawActionOccurenceCheckboxes(); 

	// Set & Get ---
	Animation* GetAnimation(); 
	void SetAnimation(Animation* newAnimation); 

	int GetCurrentFrame();
	void SetCurrentFrame(int currentFrame); 

	void SetVisible(bool newVisible);

	AnimationState animationState = ANIMATION_STOP;
	AnimationPlayMode animPlayMode = ANIMATION_LOOP; 

private:
	DisplayImageAction* screenImageAction; 

	Animation* animation = nullptr; 
	int currentFrame; 

	// Play Animation
	float animationTime; 
	bool showVariableConditions = false;
};

#endif 