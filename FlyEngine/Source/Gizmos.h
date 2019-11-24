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

	void AddaptSelectBox(FlyObject* objectAttached);

public: 
	BoundingBox* objectBorderBox;
};

class MoveGizmo
{
public: 

	MoveGizmo(FlyObject* parentObject);
	~MoveGizmo(); 

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

class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	~Gizmos();

	void Update();
	void HandleMoveGizmo();
	void HandleDrag();
	void HandleSelectionGizmo();

	// Utils -----------------
	bool IsMouseOver(); 

	// Draw ------------------
	void Draw();
	void DrawMoveGizmo();
	void DrawSelectGizmo(); 

	// Fitting ---------------
	void CalculateSelectGizmo(FlyObject* objectAttached);
	void CalculateMoveGizmo(FlyObject* objectAttached);
	void FitBoxToObject(); 
	void SetCenterSquareSize(float& centerSize);

	// Set & Get -------------
	void SetLineLength(float& lineLenght);
	void SetLineWidth(float& _arrowWidth);
	void SetArrowLenght(float& lineLenght);
	void SetArrowWidth(float& _arrowWidth);
	void SetBoxSize(float& boxSizeX, float& boxSizeY);
	void SetBoxColor(float4 color); 

	// Style -----------------
	void SetMoveGizmoStyle(float centerSize, float lineLenght, float _arrowWidth, float arrowLength, float arrowWidth, float xySquareSize);

public: 
	Transform* gizmoTransform; 

private:

	MoveGizmo* moveGizmo;
	SelectGizmo* selectGizmo; 

	FlyObject* objectAttached;
};

#endif // ! _GIZMOS_H_


