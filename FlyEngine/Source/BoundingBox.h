#ifndef  _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "MathGeoLib.h"

class FlyObject; 
class BoundingBox 
{
public: 
	BoundingBox(FlyObject* objectAttached); 
	~BoundingBox();

	virtual void Draw(bool fill, float4 color);
	virtual void CleanUp(); 

	void DrawSquare(float4 color = float4(1.0f, 1.0f, 1.0f, 0.4f), bool fill = false);

	bool IsMouseOver();
	void FitToObject(); 

	void Move(float2 newPositon);
	void CenterMinMaxPointsToScreen();

	void Rotate(float2 newRotationEuler);
	void Scale(float2 newScale);

	// Set & Get 
	void SetSize(float newMaxPointX, float newMaxPointY);
	float2& GetSize();

	void SetSquareColor(float4& newSquareColor);
	float4& GetsquareColor();

	float2& GetMinPoint();
	void SetMinPoint(float2 _minPoint); 

	float2& GetMaxPoint();
	void SetMaxPoint(float2 _maxPoint);

protected:
	float2 size;
	float4 squareColor; 

	float2 minPoint; 
	float2 maxPoint; 

	FlyObject* objectAttached; 
};

#endif // ! _2D_BOUNDING_BOX_H_

