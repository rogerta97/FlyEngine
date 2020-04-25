#ifndef _FLY_OBJECT_CHARACTER_H_
#define _FLY_OBJECT_CHARACTER_H_

#include "FlyObject.h"

#include <string>

enum CharacterMode
{
	CHARACTER_IDLE,
	CHARACTER_MOVE, 
	CHARACTER_TALK
};

class Room;
class Animation; 
class DisplayAnimationAction; 
class FlyObjectCharacter : public FlyObject
{
public: 
	FlyObjectCharacter(std::string objectName, std::string description = "", FlyObjectType _flyObjectType = ACTION_OBJECT, Room* parentRoom = nullptr);
	FlyObjectCharacter();

	bool Update(float dt);

	void SaveObjectData(JSON_Object* jsonObject, int objectIndex);

	void InitCharacter(); 
	void PlayCharacterAnimation(CharacterMode characterMode); 
	void OnSceneEnter();

	DisplayAnimationAction* GetWalkAnimation();
	DisplayAnimationAction* GetIdleAnimation();
	DisplayAnimationAction* GetTalkAnimation();

	~FlyObjectCharacter();

private: 
	DisplayAnimationAction* playingAnimation;
	CharacterMode characterMode; 

	DisplayAnimationAction* moveAnimation;
	DisplayAnimationAction* idleAnimation;
	DisplayAnimationAction* talkAnimation;
};

#endif