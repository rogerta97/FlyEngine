#ifndef _EMIT_SOUND_ACTION_H_
#define _EMIT_SOUND_ACTION_H_

#include "Action.h"

using namespace std;

class AudioClip; 
class EmitSoundAction : public Action
{
public:
	EmitSoundAction(FlyObject* _parentObject);
	~EmitSoundAction();

	void DrawUISettings(); 
	void DrawUISettingsInButton(); 

	void Play(); 

	void DrawActionOccurenceCheckboxes(); 
	void SaveAction(JSON_Object* jsonObject, std::string serializeStr, bool literalStr = false);
	void DoAction(); 

public:
	AudioClip* audioClip; 

private: 
	bool showSoundSelectionPopup = false; 
};

#endif 