#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

struct ViewportManager {

private:
	static ViewportManager* instance;
	ViewportManager();

public:
	static ViewportManager* getInstance();
	~ViewportManager();
	void Delete();

	void Draw(); 

public:

};

#endif // !_MODULE_GAME_VIEWPORT_H_


