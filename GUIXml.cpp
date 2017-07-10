/*
 * GUIXml.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#include "GUIXml.h"

//namespace TO {

GUIXml::GUIXml() {}

GUIXml::GUIXml(const char* path) {
    string inputFilesPath = path;
    inputFilesPath.append("/../data.xml");

	Log::Info("GUIXml parse %s", inputFilesPath.c_str());
	if (!Parse(inputFilesPath.c_str()))
		throw;
	Print();
}

GUIXml::~GUIXml() {
	// TODO Auto-generated destructor stub
}

ERespType GUIXml::Str2RespType(const char* s){
	if( strcmp(s,"COMPLIENCE")==0 )
		return COMPLIENCE;
	if( strcmp(s,"COMPILANCE")==0 )
		return COMPLIENCE;

	if( strcmp(s,"WEIGHTED_COMPLIANCE")==0 )
		return WEIGHTED_COMPLIENCE;
	if( strcmp(s,"WEIGHTED_COMPLIENCE")==0 )
		return WEIGHTED_COMPLIENCE;

	if( strcmp(s,"DISPLACEMENTS")==0 )
		return DISPLACEMENTS;
	if( strcmp(s,"STRESS")==0 )
		return STRESS;
	if( strcmp(s,"VIBRATION_MODE")==0 )
		return VIBRATION_MODE;
	if( strcmp(s,"CENTER_OF_GRAVITY")==0 )
		return CENTER_OF_GRAVITY;
	if( strcmp(s,"MASS")==0 )
		return MASS;
	if( strcmp(s,"VOLUME")==0 )
		return VOLUME;
	if( strcmp(s,"VOLUME_FRACTION")==0 )
		return VOLUME_FRACTION;
	return UNKNOWN;
}

const char* GUIXml::RespType2Str(ERespType t){
	switch (t){
		case COMPLIENCE: return "COMPLIENCE";
		case WEIGHTED_COMPLIENCE: return "WEIGHTED_COMPLIENCE";
		case DISPLACEMENTS: return "DISPLACEMENTS";
		case STRESS: return "STRESS";
		case VIBRATION_MODE: return "VIBRATION_MODE";
		case CENTER_OF_GRAVITY: return "CENTER_OF_GRAVITY";
		case MASS: return "MASS";
		case VOLUME: return "VOLUME";
		case VOLUME_FRACTION: return "VOLUME_FRACTION";
		default: return "UNKNOWN";
	}
}

bool GUIXml::updateLoadCaseIndex(string Id, TProperty & property){
	if(mapLoadCasesId.find(Id) == mapLoadCasesId.end()){
		Error("Response referred to unknown LC %s ", Id.c_str());
		return false;
	}
	property.LoadCaseIndex = mapLoadCasesId[Id];
	return true;
}

bool GUIXml::Parse(const char* filename){
	TiXmlDocument doc(filename);
	if (!doc.LoadFile()) {
		Error("Can't load file %s.", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlElement *pEl1, *pEl2, *pEl3;
	pEl1 = hDoc.FirstChildElement().Element();
	if (!pEl1) return false;

	TiXmlHandle hRoot(0);
	hRoot=TiXmlHandle(pEl1);
	pEl1 = hRoot.FirstChild("LoadCases").FirstChild("LoadCase").Element();
	for( ; NULL != pEl1; pEl1 = pEl1->NextSiblingElement())
	{
		TLoadCase loadCase;

		get("Id", pEl1, loadCase.Id);

		pEl2 = pEl1->FirstChildElement("LoadType");
		loadCase.isStatic = strcmp(pEl2->GetText(),"Static")==0;

		pEl2 = pEl1->FirstChildElement("FixedDomains");
		pEl2 = pEl2->FirstChildElement("FixedDomain");
		for( ; NULL != pEl2; pEl2 = pEl2->NextSiblingElement())
		{
			bVec fd;
			get("FixedAxes", pEl2, fd);
			loadCase.FixedDomains.push_back(fd);
		}

		pEl2 = pEl1->FirstChildElement("LoadDomains");
		pEl2 = pEl2->FirstChildElement("LoadDomain");
		for( ; NULL != pEl2; pEl2 = pEl2->NextSiblingElement())
		{
			TLoadDomain ld;
			if ((pEl3 = pEl2->FirstChildElement("Force"))) {

				get("Position", pEl3, ld.Position);
				get("Magnitude", pEl3, ld.Magnitude);
			}
			loadCase.LoadDomains.push_back(ld);
		}
		LoadCases.push_back(loadCase);

		// save map Id to index in LoadCases vec
		mapLoadCasesId[loadCase.Id] = LoadCases.size() - 1;
	}

	pEl1 = hRoot.FirstChild("Optimization").FirstChild("Responses").Element();
	if( NULL != pEl1 )
		pEl1 = pEl1->FirstChildElement( "Response" );
	for( ; pEl1; pEl1 = pEl1->NextSiblingElement()){
		TResponse response;

		get("Id", pEl1, response.Id);

		string sTmp;
		get("Type", pEl1, sTmp);
		response.Type = Str2RespType(sTmp.c_str());

		pEl2 = pEl1->FirstChildElement("Properties");
		if (NULL != pEl2)
			pEl2 = pEl2->FirstChildElement("Property");
		for( ; NULL != pEl2; pEl2 = pEl2->NextSiblingElement())	{
			string Id;
			TProperty property;

			if (response.Type == WEIGHTED_COMPLIENCE) {
				get("Id", pEl2, Id);
				get("Factor", pEl2, property.Factor);
			} else if (response.Type == DISPLACEMENTS) {
				get("CaseId", pEl2, Id);
				get("Direction", pEl2, property.Direction);
				get("Coordinate", pEl2, property.Coordinate);
			}else if (response.Type == VIBRATION_MODE) {
				get("VibrationModes", pEl2, property.VibrationModes);
			}

			updateLoadCaseIndex(Id, property);
			response.Properties.push_back(property);
		}


		Responses.push_back(response);
		// save map Id to index in Responses vec
		mapResponsesId[response.Id] = Responses.size() - 1;
	}

	pEl1 = hRoot.FirstChild("Optimization").FirstChild("Objective").Element();

	get("ResponseId", pEl1, Objective.ResponseId);
	get("Type", pEl1, Objective.Type);

	pEl1 = hRoot.FirstChild("Optimization").FirstChild("Constraints").Element();
	if( NULL != pEl1 )
		pEl1 = pEl1->FirstChildElement( "Constraint" );
	for( ; pEl1; pEl1 = pEl1->NextSiblingElement()){
		TConstraint constraint;

		get("ResponseId", pEl1, constraint.ResponseId);
		if(mapResponsesId.find(constraint.ResponseId) == mapResponsesId.end()){
			Error("Constraint referred to unknown Response Id %s ", constraint.ResponseId.c_str());
			return false;
		}
		constraint.ResponseIndex = mapResponsesId[constraint.ResponseId];

		get("LowerBound", pEl1, constraint.LowerBound);
		get("UpperBound", pEl1, constraint.UpperBound);

		Constraints.push_back(constraint);
	}

	pEl1 = hRoot.FirstChild("Optimization").FirstChild("Materials").Element();
	if( NULL != pEl1 )
			pEl1 = pEl1->FirstChildElement( "Material" );
	for( ; pEl1; pEl1 = pEl1->NextSiblingElement()){
		TMaterial material;

		get("name",  pEl1, material.name);
		get("value", pEl1, material.value);

		pEl2 = pEl1->FirstChildElement( "properties" );

		get("density", pEl2, material.density);
		get("modulus", pEl2, material.modulus);
		get("poisson", pEl2, material.poisson);
		get("stress",  pEl2, material.stress);
		get("key",  pEl1, material.key);

		Materials.push_back(material);
	}

	pEl1 = hRoot.FirstChild("Optimization").FirstChild("Manufacturing").Element();

	get("name",  pEl1, Manufacturing.name);

	pEl2 = pEl1->FirstChildElement( "properties" );
	get("x", pEl2, Manufacturing.x);
	get("y", pEl2, Manufacturing.y);
	get("z", pEl2, Manufacturing.z);
	get("angle", pEl2, Manufacturing.angle);

	pEl1 = hRoot.FirstChild("Optimization").Element();
	get("Resolution",  pEl1, Resolution);
	return true;
}

void GUIXml::printVec(string name, dVec &vec){
	Log::Print("%s vec: x:%f; y:%f; z:%f.", name.c_str(), vec.x, vec.y, vec.z );
}

void GUIXml::printVec(string name, bVec &vec){
	Log::Print("%s vec: x:%i; y:%i; z:%i.", name.c_str(), vec.x, vec.y, vec.z );
}

void GUIXml::PrintLC(TLoadCase lc){
	Log::Print("\t Id", lc.Id );
    for(unsigned int i = 0; i < lc.FixedDomains.size(); i++) {
    	Log::Print("FixedDomain", i );
		printVec("\t FixedAxes", lc.FixedDomains[0]);
    }
    for(unsigned int i = 0; i < lc.LoadDomains.size(); i++) {
    	Log::Print("LoadDomain", i );
		printVec("\t Position", lc.LoadDomains[0].Position);
		printVec("\t Magnitude", lc.LoadDomains[0].Magnitude);
    }
}

void GUIXml::PrintRespProperty(ERespType type, TProperty property){
	if (type == WEIGHTED_COMPLIENCE) {
		Log::Print("\t Factor", property.Factor);
		Log::Print("\t LoadCaseIndex", property.LoadCaseIndex);
		//printLC(LoadCases[Responses[i].Properties[j].LoadCaseIndex]);
	} else if (type == DISPLACEMENTS) {
		Log::Print("\t Direction", property.Direction);
		printVec("\t Coordinate", property.Coordinate);
		Log::Print("\t LoadCaseIndex", property.LoadCaseIndex);
	}else if (type == VIBRATION_MODE) {
		Log::Print("\t VibrationModes", property.VibrationModes);
	}
}

void GUIXml::Print(){
	Log::Print("* * *   Print xml data   * * *\n");

    for(unsigned int i = 0; i < LoadCases.size(); i++) {
    	Log::Print("LoadCases", i );
    	PrintLC(LoadCases[i]);
    }

    for(unsigned int i = 0; i < Responses.size(); i++) {
    	Log::Print("Response", i );
    	Log::Print("\t Type", RespType2Str(Responses[i].Type));
    	Log::Print("\t Id", Responses[i].Id );
        for(unsigned int j = 0; j < Responses[i].Properties.size(); j++) {
        	Log::Print("Properties", j );
        	PrintRespProperty(Responses[i].Type, Responses[i].Properties[j]);
        }
    }

    for(unsigned int i = 0; i < Constraints.size(); i++) {
    	Log::Print("Constraint", i );
    	Log::Print("\t ResponseId", Constraints[i].ResponseId );
    	Log::Print("\t ResponseIndex", Constraints[i].ResponseIndex );
    	Log::Print("\t LowerBound", Constraints[i].LowerBound );
    	Log::Print("\t UpperBound", Constraints[i].UpperBound );
    }

    for(unsigned int i = 0; i < Materials.size(); i++) {
    	Log::Print("Material", i );
    	Log::Print("\t name", Materials[i].name );
    	Log::Print("\t value", Materials[i].value );
    	Log::Print("\t density", Materials[i].density );
    	Log::Print("\t poisson", Materials[i].poisson );
    	Log::Print("\t stress", Materials[i].stress );
    	Log::Print("\t key", Materials[i].key );
    }

    Log::Print("Manufacturing:");
	Log::Print("\t name", Manufacturing.name );
	Log::Print("\t x", Manufacturing.x );
	Log::Print("\t y", Manufacturing.y );
	Log::Print("\t z", Manufacturing.z );
	Log::Print("\t angle", Manufacturing.angle );

	Log::Print("Resolution", Resolution );

	Log::Print("* * *  End of print xml data   * * *\n");
}

//} /* namespace TO */
