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
	void DrawSquare(math::float2& topLeft, math::float2& topRight, math::float2& bottomLeft, math::float2& bottomRight, float4 color);
	void DrawControlPoint(float2 pointPos, float pointSize);

	void FitToObject(); 

	void SetSize(float newMaxPointX, float newMaxPointY);
	float2& GetSize();

	void SetSquareColor(float4& newSquareColor);
	float4& GetsquareColor();

	void ShowCornerDots(bool show);

private: 

	bool showCornerDots; 

	float2 size;
	float4 squareColor; 

	FlyObject* objectAttached; 
};

#endif // ! _2D_BOUNDING_BOX_H_

