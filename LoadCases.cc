/*
 * LoadCases.cpp
 *
 *  Created on: May 24, 2017
 *      Author: user
 */

#include "LoadCases.h"

#include <sys/types.h>
#include <dirent.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include "string.h"


std::vector<std::string> split(const std::string& str, const std::string& delim){
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

static int RHSfilter(const struct dirent* dir_ent){
    if (!strcmp(dir_ent->d_name, ".") || !strcmp(dir_ent->d_name, "..")) return 0;
    std::string fname = dir_ent->d_name;
    if (fname.find("RHS") == std::string::npos) return 0;
    return 1;
}


LoadCases::LoadCases(const char* inputPath) {
    Init(inputPath);
}

LoadCases::~LoadCases() {

}

PetscErrorCode LoadCases::Init(const char* inputPath){

    std::string inputFilesPath = inputPath;
    inputFilesPath.append("/optimization_data.txt");
    Debug("%s", inputFilesPath.c_str());

    std::string tmpstr;
    std::fstream instr;
    instr.open(inputFilesPath.c_str(),std::ios::in);
    while (std::getline(instr, tmpstr)){
        instr>>tmpstr;
        std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), ::tolower);
        if(tmpstr=="nbc"){
        	instr>>nbc;
		}
		else if (tmpstr=="nlc"){
            tnlc = 0;
            for (int i=0; i<nbc; i++){
                instr>>nlc[i];
                tnlc = tnlc + nlc[i];
            }
        }
        else {
            continue;
        }
    }
    instr.close();


	{
        int bcnum;
        int rhsnum;

        // Find all RHS*.bin files in input, organize i_j_k indices
        struct dirent **namelist;
        int nfiles = scandir(inputPath, &namelist, *RHSfilter, alphasort);
        if (nfiles < 0)
            perror("scandir");
        else {
            while (nfiles--) {
                std::string filestr(namelist[nfiles]->d_name);
                std::string delim="_";
                std::vector<std::string> rez = split(filestr,delim);
                bcnum = atoi(rez[1].c_str());
                rhsnum = atoi(rez[2].c_str());
                ijkmat[bcnum-1][rhsnum-1] = atoi(rez[3].c_str());
                Debug("Found BC %i and RHS %i and LC %i", bcnum, rhsnum, ijkmat[bcnum-1][rhsnum-1]);
                free(namelist[nfiles]);
            }
            free(namelist);
        }
    }
/*
    // Read in RHS one by one
    Vec inputvec;
    PetscViewer viewer1;
    MPI_Comm comm = PETSC_COMM_WORLD;

    ierr = DMDACreateNaturalVector(pMesh->GetNDMDA(),&inputvec); CHKERRQ(ierr);  // Read in natural ordering

    for (int i=0; i<pBC->nbc; i++){
        for (int j=0; j<nlc[i]; j++){
            int lcnum = ijkmat[i][j];
            // Read RHS one by one for this BC
            ierr = PetscViewerBinaryOpen(comm, TOData::GetInputFilePath("RHS_%i_%i_%i.bin", i+1, j+1, lcnum). c_str(),
                    FILE_MODE_READ,&viewer1); CHKERRQ(ierr); // Read binary file with RHS data
            ierr = VecLoad(inputvec,viewer1); CHKERRQ(ierr); // Keep RHS data in new natural vector
            ierr = DMDANaturalToGlobalBegin(pMesh->GetNDMDA(),inputvec,INSERT_VALUES,RHS[lcnum-1]); CHKERRQ(ierr); // Transfer vector to global ordering - start
            ierr = DMDANaturalToGlobalEnd(pMesh->GetNDMDA(),inputvec,INSERT_VALUES,RHS[lcnum-1]); CHKERRQ(ierr); // Transfer vector to global ordering - end
            VecSet(inputvec,0.0);
        }
    }

    ierr = PetscViewerDestroy(&viewer1); CHKERRQ(ierr); // Destroy viewer
*/
    return 0;
}

