#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

struct JSONParser {

private:
	static JSONParser* instance;
	JSONParser();

public:
	static JSONParser* getInstance();
	~JSONParser();

	static void Delete();
};

#endif 


