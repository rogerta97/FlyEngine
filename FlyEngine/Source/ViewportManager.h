#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

class TextureMSAA; 
struct ViewportManager {

private:
	static ViewportManager* instance;
	ViewportManager();

public:
	static ViewportManager* getInstance();
	~ViewportManager();
	void Delete();

	void DrawRoomViewport(); 

public:
	TextureMSAA* viewportTexture;

};

#endif // !_MODULE_GAME_VIEWPORT_H_


