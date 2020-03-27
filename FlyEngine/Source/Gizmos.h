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
	SelectGizmo(); 
	~SelectGizmo(); 

	void CleanUp();

	void AddaptSelectBox(FlyObject* objectAttached);

public: 
	BoundingBox* objectBorderBox;
};

class MoveGizmo
{
public: 

	MoveGizmo();
	~MoveGizmo(); 

	void CleanUp();
	void AddaptAxisBoxes(FlyObject* objectAttached);
	void AddaptAxisBoxes(BoundingBox* boxAttached);

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

	ScaleGizmo();
	~ScaleGizmo();

	void CleanUp(); 
	void AddaptScaleBox(FlyObject* objectAttached);
	void AddaptAxisBoxes(FlyObject* objectAttached);
 
public: 
	float lineLengthX;
	float lineLengthY;

	float2 endAxisBoxXPos = float2::zero;
	float2 endAxisBoxYPos = float2::zero;
	float2 endAxisBoxXYPos = float2::zero;

	float2 initDragEndBoxXPos = float2::zero;
	float2 initDragEndBoxYPos = float2::zero; 
	float2 initDragEndBoxXYPos = float2::zero; 

	float lineWidth = 0; 
	float lineLength = 0; 

	float lineSquareSize = 0;
	float centerSquareSize = 0;

	float xySquareSize = 0;
	float2 xySquarePos = float2::zero;

	BoundingBox* axisXBox;
	BoundingBox* axisYBox;
	BoundingBox* axisXYBox;

	BoundingBox* borderBoundingBox; 
};

class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	Gizmos(BoundingBox* boxAttached);

	~Gizmos();

	bool Update();
	bool HandleScaleGizmo();
	bool HandleMoveGizmo();
	void HandleDrag(float dragMovementIncX, math::float2& inc, float& prevPosX, float aspectRatio, float dragMovementIncY, float& prevPosY, math::float2& dragMovementIncXY, math::float2& prevPos);
	void CleanUp(); 

	// Utils -----------------
	bool IsMouseOver(); 
	bool IsMouseOverClicked(); 

	// Draw ------------------
	void Draw();
	void DrawSelectGizmo(); 
	void DrawMoveGizmo();
	void DrawScaleGizmo();

	// Fitting ---------------
	void CalculateSelectGizmo(FlyObject* objectAttached);
	void CalculateMoveGizmo(FlyObject* objectAttached);
	void CalculateMoveGizmo(BoundingBox* boxAttached);
	void CalculateScaleGizmo(FlyObject* objectAttached);

	void FitSelectBoxSize(); 
	void FitScaleBoxSize();
	void SetCenterSquareSize(float& centerSize);

	// Style -----------------
	void SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth, float arrowLength, float arrowWidth, float xySquareSize);
	void SetScaleGizmoStyle(float centerSize, float lineLenght, float _arrowWidth, float arrowLength, float arrowWidth, float xySquareSize);

	SelectGizmo*	selectGizmo; 
	MoveGizmo*		moveGizmo;
	ScaleGizmo*		scaleGizmo; 

private: 
	FlyObject*		objectAttached;
	BoundingBox*	boxAttached; 
};

#endif // ! _GIZMOS_H_


