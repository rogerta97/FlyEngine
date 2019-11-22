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

	void DrawSquare(float4 color);
	void DrawControlPoint(float2 pointPos, float pointSize);
	void DrawMinAndMaxPoints(); 

	bool IsMouseOver();
	void FitToObject(); 

	void Move(float2 newPositon);
	void CenterMinMaxPoints();

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

public:
	bool showCornerDots; 

private: 
	float2 size;
	float4 squareColor; 

	float2 minPoint; 
	float2 maxPoint; 

	FlyObject* objectAttached; 
};

#endif // ! _2D_BOUNDING_BOX_H_

