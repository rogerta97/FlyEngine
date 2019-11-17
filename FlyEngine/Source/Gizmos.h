#ifndef _GIZMOS_H_
#define _GIZMOS_H_

#include "MathGeoLib.h"

class FlyObject;
class Gizmos
{
public:
	Gizmos(FlyObject* objectAttached);
	~Gizmos();

	void Draw();
	void DrawMoveGizmo();

	void SetCenterSquareSize(float& centerSize); 
	void SetArrowLenght(float& lineLenght);
	void SetArrowWidth(float& _arrowWidth);

	void SetGizmoStyle(float centerSize, float lineLenght, float _arrowWidth); 

private:

	float arrowLenght; 
	float arrowWidth; 
	float centerSquareSize; 

	FlyObject* objectAttached;
};

#endif // ! _GIZMOS_H_


