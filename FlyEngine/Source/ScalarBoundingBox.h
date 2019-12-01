#ifndef _SCALAR_BOUNDING_BOX_H_
#define _SCALAR_BOUNDING_BOX_H_

#include "BoundingBox.h"

class ScalarBoundingBox : public BoundingBox
{
public:
	ScalarBoundingBox(FlyObject* objectAttached); 
	~ScalarBoundingBox();

	void Draw(bool fill, float4 color); 
	void Update();
	void CleanUp(); 

	void SetCornerBoxSize(float newSize);

public: 
	bool drawCornerPoints = false; 

private:
	
	float cornerBoxesSize; 

	BoundingBox* topLeftScaleBox; 
	BoundingBox* topRightScaleBox; 
	BoundingBox* bottomLeftScaleBox; 
	BoundingBox* bottomRightScaleBox; 


};

#endif

