#ifndef _SAVE_AND_LOAD_H_
#define _SAVE_AND_LOAD_H_

#include "JSON/parson.h"

struct SaveAndLoad {

private:
	static SaveAndLoad* instance;
	SaveAndLoad();

public:
	static SaveAndLoad* getInstance();
	~SaveAndLoad();

	void SaveCurrentRoom();

	static void Delete();
};

#endif 


