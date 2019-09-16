#ifndef  _UID_FACTORY_H_
#define _UID_FACTORY_H_

class UIDFactory {

private:
	static UIDFactory* instance;
	UIDFactory() {}; 

public:
	static UIDFactory* getInstance() {
		if (instance == 0) instance = new UIDFactory();		
		return instance; 
	}
	~UIDFactory() {}; 
};

#endif // ! _UID_FACTORY_H_

