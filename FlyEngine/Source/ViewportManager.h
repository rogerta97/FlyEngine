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
	void Delete();
	void ResizeViewport(); 

	float GetWidthFromHeight(float viewportHeight); 
	float GetHeightFromWidth(float viewportWidth);
	void SetTextureSize(float windowWidth, float windowHeight) const;

	// Encapsulation --------------------------
	ViewportAspectRatio GetAspectRatioType() const; 
	void SetAspectRatioType(ViewportAspectRatio newAR); 

public:
	TextureMSAA* viewportTexture;
	ViewportAspectRatio viewportAspectRatio; 

};

#endif // !_MODULE_GAME_VIEWPORT_H_


