#ifndef _MODULE_AUDIOMANAGER_H_
#define _MODULE_AUDIOMANAGER_H_

#include "Module.h"

class ModuleAudioManager : public Module
{
public:

	ModuleAudioManager(bool start_enabled = true);
	~ModuleAudioManager();

	bool Init();
	bool CleanUp(); 

};

#endif
