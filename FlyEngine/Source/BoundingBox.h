#ifndef  _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "MathGeoLib.h"

class FlyObject; 
class Gizmos; 
class BoundingBox 
{
public: 
	BoundingBox(); 
	~BoundingBox();

	bool UpdateGizmos(); 
	void CalculateAllGizmos();
	virtual void Draw(bool fill, float4 color);
	virtual void DrawBoxGizmos();
	virtual void CleanUp();

	// Draw ----------
	void DrawSquare(float4 color = float4(1.0f, 1.0f, 1.0f, 0.4f), bool fill = false);

	// Utility -------
	float2 GetCenter(); 
	float2 GetTopLeft(); 
	void CenterMinMaxPointsToScreen();
	void EnableDrag(bool enable); 
	void CreateGizmos();

	//Input ----------
	bool IsMouseOver();
	bool IsBoxClicked();
	float2 HandleDrag(CardinalAxis limitOnAxis = CardinalAxis::AxisNone);

	// Set & Getc 
	void SetPosition(float2 newPositon, bool fromTopLeft = false);
	void SetPositionInc(float2 newPositon); 
	float2 GetPosition();

	void SetSize(float newMaxPointX, float newMaxPointY);
	float2& GetSize();

	void SetSquareColor(float4 newSquareColor);
	float4& GetSquareColor();

	float2& GetMinPoint();
	void SetMinPoint(float2 _minPoint); 

	Gizmos* GetBoxGizmos();

	float2& GetMaxPoint();
	void SetMaxPoint(float2 _maxPoint);

	bool& IsDragging();


protected:
	bool displayGizmos; 
	Gizmos* boxGizmos; 

	float2 size;
	float4 squareColor; 

	float2 minPoint; 
	float2 maxPoint; 

	FlyObject* objectAttached; 

	// Drag -----------
	bool isDragEnabled = false;
	bool isDragging = false;

	float2 initDragPos = float2::zero; 
	float2 initDragMinPoint = float2::zero; 
	float2 initDragMaxPoint = float2::zero; 
};

#endif // ! _2D_BOUNDING_BOX_H_

