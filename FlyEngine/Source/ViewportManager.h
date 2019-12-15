#ifndef _VIEWPORT_MANAGER_H_
#define _VIEWPORT_MANAGER_H_

#include "glm.hpp"
#include "Math/float2.h"
#include <list>

using namespace glm; 
using namespace std; 

enum ViewportAspectRatio
{
	AR_4_3,
	AR_1_1,
};

class TextureMSAA; 
class FlyObject; 
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
	static list<FlyObject*> RaycastMouseClick(); 

	static float GetWidthFromHeight(float viewportHeight);
	static float GetHeightFromWidth(float viewportWidth);
	static void SetTextureSize(float windowWidth, float windowHeight);

	// Aspect Ratio
	static float2& ScreenToWorld(float2 screenPos);
	static float2& ScreenToWorld(float screenPosX, float screenPosY);
	static float GetAspectRatio();

	// Encapsulation --------------------------
	static ViewportAspectRatio GetAspectRatioType();
	static void SetAspectRatioType(ViewportAspectRatio newAR);

public:
	TextureMSAA* viewportTexture;
	ViewportAspectRatio viewportAspectRatio; 

	// Render Viewport Settings 
	bool drawGizmos;

};

#endif // !_MODULE_GAME_VIEWPORT_H_


