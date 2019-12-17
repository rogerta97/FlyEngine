#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "MathGeoLib.h"

class BoundingBox; 
class FlyObject;
class Transform;

enum DragAxis
{
	DRAG_X,
	DRAG_Y,
	DRAG_XY,
	NOT_DRAG
};

class SelectGizmo
{
public: 
	SelectGizmo(FlyObject* parentObject); 
	~SelectGizmo(); 

	void CleanUp();

	void AddaptSelectBox(FlyObject* objectAttached);

public: 
	BoundingBox* objectBorderBox;
};

class MoveGizmo
{
public: 

	MoveGizmo(FlyObject* parentObject);
	~MoveGizmo(); 

	void CleanUp();
	void AddaptAxisBoxes(FlyObject* objectAttached);

public:
	float lineLength;
	float lineWidth;

	float arrowLength;
	float arrowWidth; 

	float xySquareSize;
	float2 xySquarePos; 

	BoundingBox* axisXBox;
	BoundingBox* axisYBox;
	BoundingBox* axisXYBox;

	DragAxis dragAxis;
	float2 dragCenterOffset; 
	float2 initDragPos; 

	float centerSquareSize;
};

class ScaleGizmo
{
public:

	ScaleGizmo(FlyObject* parentObject);
	~ScaleGizmo();

	void AddaptScaleBox(FlyObject* objectAttached);
 
public: 

	float lineWidth; 
	float lineLength; 

	float lineSquareSize;
	float centerSquareSize;

	float xySquareSize;
	float2 xySquarePos;

	BoundingBox* borderBoundingBox; 
};

class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	~Gizmos();

	void Update();
	void HandleMoveGizmo();
	void HandleDrag(float dragMovementIncX, math::float2& inc, float& prevPosX, float aspectRatio, float dragMovementIncY, float& prevPosY, math::float2& dragMovementIncXY, math::float2& prevPos);
	void CleanUp(); 

	// Utils -----------------
	bool IsMouseOver(); 

	// Draw ------------------
	void Draw();
	void DrawSelectGizmo(); 
	void DrawMoveGizmo();
	void DrawScaleGizmo();

	// Fitting ---------------
	void CalculateSelectGizmo(FlyObject* objectAttached);
	void CalculateMoveGizmo(FlyObject* objectAttached);
	void CalculateScaleGizmo(FlyObject* objectAttached);

	void FitSelectBoxSize(); 
	void FitScaleBoxSize();
	void SetCenterSquareSize(float& centerSize);

	// Style -----------------
	void SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth, float arrowLength, float arrowWidth, float xySquareSize);
	void SetScaleGizmoStyle(float centerSize, float lineLenght, float _arrowWidth, float arrowLength, float arrowWidth, float xySquareSize);

	SelectGizmo* selectGizmo; 
	MoveGizmo* moveGizmo;
	ScaleGizmo* scaleGizmo; 

private: 
	FlyObject* objectAttached;
};

#endif // ! _GIZMOS_H_


