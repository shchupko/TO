/*
 * ParseXML.h
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#ifndef PARSEXML_H_
#define PARSEXML_H_


#include "Log.h"
#ifdef TINY2
	#include "tinyxml2.h"
	#define TiXmlDocument tinyxml2::XMLDocument
	#define TiXmlElement tinyxml2::XMLElement
	#define TiXmlHandle tinyxml2::XMLHandle
#else
	#include "tinyxml.h"
#endif

using namespace std;


struct dVec{
	double x, y, z;
};

struct bVec{
	bool x, y, z;
};

class ParseXML : protected Log::Log {
public:
	ParseXML();
	virtual ~ParseXML();
	virtual void Print(void){};
	virtual bool Parse(const char* filename){ return false; };

	const char* getText(const char* value, TiXmlElement* node);
	void get(const char* value, TiXmlElement* node, string & retVal);

	double getDouble(const char* value, TiXmlElement* node);
	void get(const char* value, TiXmlElement* node, double & retVal);
	void get(const char* value, TiXmlElement* node, int & retVal);

	bool getBool(const char* value, TiXmlElement* node);
	void get(const char* value, TiXmlElement* node, bool & retVal);

	void get(const char* value, TiXmlElement* node, struct dVec & vec);
	void get(const char* value, TiXmlElement* node, struct bVec & vec);
};

#endif /* PARSEXML_H_ */
