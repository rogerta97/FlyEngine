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
	void SetArrowLenght(float& lineLenght);
	void SetArrowWidth(float& _arrowWidth);

private:

	float arrowLenght; 
	float arrowWidth; 

	FlyObject* objectAttached;
};

#endif // ! _GIZMOS_H_


