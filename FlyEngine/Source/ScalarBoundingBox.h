#ifndef _SCALAR_BOUNDING_BOX_H_
#define _SCALAR_BOUNDING_BOX_H_

#include "BoundingBox.h"

class ScalarBoundingBox : public BoundingBox
{
public:
	ScalarBoundingBox(FlyObject* objectAttached, bool drawCornerPoints = false); 
	~ScalarBoundingBox();

	void Draw(bool fill, float4 color); 
	void Update();
	void CleanUp(); 

	void SetCornerBoxSize(float newSize);
	void EnableScaling(bool enableScaling); 

public: 
	bool drawCornerPoints = false; 

private:	
	float cornerBoxesSize; 
	bool scalingEnabled; 

	BoundingBox* topLeftScaleBox; 
	BoundingBox* topRightScaleBox; 
	BoundingBox* bottomLeftScaleBox; 
	BoundingBox* bottomRightScaleBox; 
};

#endif

