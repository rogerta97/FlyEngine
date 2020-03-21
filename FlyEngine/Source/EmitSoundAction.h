#ifndef _EMIT_SOUND_ACTION_H_
#define _EMIT_SOUND_ACTION_H_

#include "Action.h"

using namespace std;

enum AudioPlayMode
{
	ONE_TIME, 
	AUDIO_LOOP,
};

class AudioClip; 
class EmitSoundAction : public Action
{
public:
	EmitSoundAction(FlyObject* _parentObject);
	~EmitSoundAction();

	void Update(float dt); 

	void DrawUISettings(); 
	void DrawUISettingsInButton(); 

	void DrawActionOccurenceCheckboxes(); 
	
	void Play(); 
	void Stop(); 

	void SaveAction(JSON_Object* jsonObject, std::string serializeStr, bool literalStr = false, int actionPositionInObject = 0);
	void DoAction(); 

public:
	AudioClip* audioClip; 
	AudioPlayMode audioPlayMode; 

private: 
	bool showVariableConditions = false;

	bool showSoundSelectionPopup = false; 
	bool isPlaying = false; 
	int audioChannel = -1; 
};

#endif 