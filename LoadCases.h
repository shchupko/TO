/*
 * LoadCases.h
 *
 *  Created on: May 24, 2017
 *      Author: user
 */

#ifndef LOADCASES_H_
#define LOADCASES_H_

#include "MockPetsc.h"
#include <vector>
#include "Log.h"

//namespace LoadCases {



class LoadCases : Log::Log {
public:
    LoadCases(const char* InputPath);
    ~LoadCases();

    PetscInt nbc; // Number of BCs
    PetscInt nlc[8]; // Number of load cases per BC (max 8 BCs)
    PetscInt tnlc; // Total number of load cases

    int ijkmat[20][20];
    /*vector<vector<int>> vec;
    vec.reserve(3);
    for (int i=0, size=vec.size(); i<size; ++i)
        vec[i].reserve(5);
    vec[i][j]= 1;  // 3x5*/

private:
    PetscErrorCode Init(const char* InputPath);
};

//} /* namespace LoadCases */

#endif /* LOADCASES_H_ */
