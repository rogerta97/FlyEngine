#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

#include "glm.hpp"

using namespace glm; 

enum ViewportAspectRatio
{
	AR_4_3,
};

class TextureMSAA; 
struct ViewportManager 
{
private:
	static ViewportManager* instance;
	ViewportManager();

public:
	static ViewportManager* getInstance();
	~ViewportManager();
	
	// Utility --------------------------------
	void Delete();
	void ResizeViewport(); 
	void DrawRoomViewport(); 

	float GetWidthFromHeight(float viewportHeight); 
	float GetHeightFromWidth(float viewportWidth);

	// Encapsulation --------------------------
	ViewportAspectRatio GetAspectRatio() const; 
	void SetAspectRatio(ViewportAspectRatio newAR); 

public:
	TextureMSAA* viewportTexture;
	ViewportAspectRatio viewportAspectRatio; 

};

#endif // !_MODULE_GAME_VIEWPORT_H_


