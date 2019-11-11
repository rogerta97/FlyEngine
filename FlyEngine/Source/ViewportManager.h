#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

#include "glm.hpp"

using namespace glm; 

enum ViewportAspectRatio
{
	AR_4_3,
	AR_1_1,
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
	static void Delete();
	static void ResizeViewport();

	static float GetWidthFromHeight(float viewportHeight);
	static float GetHeightFromWidth(float viewportWidth);
	static void SetTextureSize(float windowWidth, float windowHeight);

	// Encapsulation --------------------------
	static ViewportAspectRatio GetAspectRatioType();
	static void SetAspectRatioType(ViewportAspectRatio newAR);

public:
	TextureMSAA* viewportTexture;
	ViewportAspectRatio viewportAspectRatio; 

};

#endif // !_MODULE_GAME_VIEWPORT_H_


