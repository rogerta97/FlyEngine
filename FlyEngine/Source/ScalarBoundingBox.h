#ifndef _SCALAR_BOUNDING_BOX_H_
#define _SCALAR_BOUNDING_BOX_H_

#include "BoundingBox.h"

class ScalarBoundingBox : public BoundingBox
{
	ScalarBoundingBox(FlyObject* objectAttached); 
	~ScalarBoundingBox();

	void Draw(bool fill, float4 color); 
	void CleanUp(); 

private: 

};

#endif

