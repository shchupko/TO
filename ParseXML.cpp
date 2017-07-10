/*
 * ParseXML.cpp
 *
 *  Created on: Jun 12, 2017
 *      Author: user
 */

#include <typeinfo>
#include <stdlib.h>  //atof
#include "ParseXML.h"


ParseXML::ParseXML() {}
ParseXML::~ParseXML() {}

const char* ParseXML::getText(const char* value, TiXmlElement* node){
	TiXmlElement *pEl = node->FirstChildElement( value );
	return pEl->GetText();
}

// unused
double ParseXML::getDouble(const char* value, TiXmlElement* node){
	return atof(getText(value, node));
}

void ParseXML::get(const char* value, TiXmlElement* node, string & retVal){
	retVal = getText(value, node);
}

void ParseXML::get(const char* value, TiXmlElement* node, double & retVal){
	retVal = atof(getText(value, node));
}

void ParseXML::get(const char* value, TiXmlElement* node, bool & retVal){
	retVal = strcmp(getText(value, node),"true")==0;
}


void ParseXML::get(const char* value, TiXmlElement* node, struct dVec & vec){
	TiXmlElement* nodeTmp = node->FirstChildElement( value );
	if(nodeTmp != NULL ){
		get("X", nodeTmp, vec.x);
		get("Y", nodeTmp, vec.y);
		get("Z", nodeTmp, vec.z);
	}
}

void ParseXML::get(const char* value, TiXmlElement* node, struct bVec & vec){
	TiXmlElement* nodeTmp = node->FirstChildElement( value );
	if(nodeTmp != NULL ){
		get("X", nodeTmp, vec.x);
		get("Y", nodeTmp, vec.y);
		get("Z", nodeTmp, vec.z);
	}
}

// unused
void ParseXML::get(const char* value, TiXmlElement* node, int & retVal){
	retVal = atoi(getText(value, node));
}

bool atob (const char *s){
	return s && strcmp(s,"true")==0;
}
// unused
bool ParseXML::getBool(const char* value, TiXmlElement* node){
	return atob(getText(value, node));
}
