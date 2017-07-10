/*
 * GUIXml.h
 *
 *  Created on: Jun 14, 2017
 *      Author: user
 */

#ifndef GUIXML_H_
#define GUIXML_H_

#include <vector>
#include <map>
#include "Log.h"
#include "ParseXML.h"

//namespace TO {


typedef struct {
	dVec Position;
	dVec Magnitude;
}TLoadDomain;

//typedef bVec TFixedDomain;

typedef struct {
    string Id;  //
    bool isStatic; //<LoadType> Static or Vibration
    vector<bVec> FixedDomains;
    vector<TLoadDomain> LoadDomains;
}TLoadCase;

typedef enum {
	UNKNOWN,
	COMPLIENCE,
	WEIGHTED_COMPLIENCE,
	DISPLACEMENTS,
	STRESS,
	VIBRATION_MODE,
	CENTER_OF_GRAVITY,
	MASS,
	VOLUME,
	VOLUME_FRACTION
} ERespType;

typedef struct {
    //string Id; // for development period only, for info, will be removed
    unsigned int LoadCaseIndex;

    // DISPLACEMENTS
    string Direction;
    dVec Coordinate;

    // WEIGHTED_COMPLIENCE
    double Factor;

    // VIBRATION_MODE
    double VibrationModes;  // ? int
}TProperty;

typedef struct {
    string Id;
    ERespType Type;
    vector <TProperty> Properties;
}TResponse;

typedef enum {Min, Max} EObjectiveType;
typedef struct {
    string ResponseId;
    string Type; //EObjectiveType Type;
}TObjective;

typedef struct {
    string ResponseId; // temporary, after stabilization will be used only ResponseIndex
    unsigned int ResponseIndex;
    double LowerBound;
    double UpperBound;
}TConstraint;

typedef struct {
    string name;
    string value;
    double density;
    double modulus;
    double poisson;
    double stress;
    string key;
}TMaterial;

typedef enum {Low, Medium, High} EResolution;

typedef struct {
    string name;
    double x;
    double y;
    double z;
    double angle;
}TManufacturing;

class GUIXml : public ParseXML {
public:
	GUIXml();
	GUIXml(const char* pFilename);
	~GUIXml();

	void Print(void);
    bool Parse(const char* filename);

    ERespType Str2RespType(const char* s);
    const char* RespType2Str(ERespType t);

    void PrintLC(TLoadCase lc);
    void PrintRespProperty(ERespType type, TProperty property);

	vector<TLoadCase> LoadCases;
	vector<TResponse> Responses;
    TObjective Objective;

    vector<TConstraint> Constraints;
    vector<TMaterial> Materials;
    TManufacturing Manufacturing;
    string Resolution; // EResolution have

    map<string, unsigned int> mapLoadCasesId; // map LoadCase Id to index in LoadCases vec
    map<string, unsigned int> mapResponsesId; // map R. Id to index in Responses vec
   // map<unsigned int, unsigned int> indexRespByLC; //  get Response index in Responses[] by LC index in LoadCases[]

private:
    void printVec(string name, dVec &vec);
    void printVec(string name, bVec &vec);
    bool updateLoadCaseIndex(string Id, TProperty & property);

};

//} /* namespace TO */

#endif /* GUIXML_H_ */
