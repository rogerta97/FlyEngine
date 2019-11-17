#ifndef  _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "MathGeoLib.h"

class FlyObject; 
class BoundingBox 
{
public: 
	BoundingBox(FlyObject* objectAttached); 
	~BoundingBox();

	void Draw(); 
	void FitToObject(); 

	void SetMinPoint(float newMinPointX, float newMinPointY);
	float2& GetMinPoint();

	void SetMaxPoint(float newMaxPointX, float newMaxPointY);
	float2& GetMaxPoint();

	void SetSize(float newMaxPointX, float newMaxPointY);
	float2& GetSize();

private: 
	float2 size;
	float2 minPoint; 
	float2 maxPoint;
	FlyObject* objectAttached; 
};

#endif // ! _2D_BOUNDING_BOX_H_

