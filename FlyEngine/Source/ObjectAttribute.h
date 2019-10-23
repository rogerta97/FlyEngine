#ifndef _OBJECT_ATTRIBUTE_H_
#define _OBJECT_ATTRIBUTE_H_

enum AttributeType 
{
	AT_IMAGE, 
	AT_null, 
};

class ObjectAttribute
{
public:
	ObjectAttribute(); 
	~ObjectAttribute();

	virtual void Init(); 
	virtual void Update(); 
	virtual void Draw(); 
	virtual void CleanUp(); 

public:
	AttributeType attributeType;
};

#endif

