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

public:
	AudioClip* audioClip; 
};

#endif 