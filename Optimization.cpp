/*
 * Optimization.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: user
 */

#include "Optimization.h"

//namespace TO {

Optimization::Optimization() {}
Optimization::~Optimization() {}

void Optimization::Loop(LoadCases & LC, GUIXml & GUIXml){

	unsigned int maxItr = 2;

	Info("\n");
	for(unsigned int itr = 0; itr < maxItr; itr++)
	{
		Info("\n* * *  Optimization loop. Itr %i. * * *", itr);

		for (int i = 0; i < LC.nbc; i++) {

			Debug("for BC_%i.bin Construct stiffness matrix", i + 1);
			for (int j = 0; j < LC.nlc[i]; j++) {

				unsigned int lcnum = LC.ijkmat[i][j];
				// Read RHS one by one for this BC
				Debug("   for RHS_%i_%i_%i.bin", i + 1, j + 1, lcnum);

				for(unsigned int r = 0; r < GUIXml.Responses.size(); r++) {
					if (GUIXml.Responses[r].Properties.size() == 0) {
						Debug("     Skip (No property). Responses[%i], Type %s, Id %s", r,
								GUIXml.RespType2Str(GUIXml.Responses[r].Type),
								GUIXml.Responses[r].Id.c_str());
						continue;
					}

					for(unsigned int p = 0; p < GUIXml.Responses[r].Properties.size(); p++){
						if( (GUIXml.Responses[r].Properties[p].LoadCaseIndex + 1) != lcnum){
							Debug("     Skip (Other LC). Responses[%i], Type %s, Id %s", r,
									GUIXml.RespType2Str(GUIXml.Responses[r].Type), GUIXml.Responses[r].Id.c_str());
							continue;
						}
						else{
							Info("     Use Responses[%i], Type %s", r, GUIXml.RespType2Str(GUIXml.Responses[r].Type));
							GUIXml.PrintRespProperty(GUIXml.Responses[r].Type, GUIXml.Responses[r].Properties[p]);
							GUIXml.PrintLC(GUIXml.LoadCases[GUIXml.Responses[r].Properties[p].LoadCaseIndex]);

							if (GUIXml.LoadCases[lcnum].isStatic) {
								Info("       Construct adjoint Vector and solve K%i*LAM%i%i = D%i%i",
										i + 1, i + 1, lcnum, i + 1, lcnum);
								Info("       use BC_%i.bin and RHS_%i_%i_%i.bin", i + 1, i + 1, j + 1, lcnum);
							}
							else {
								Info("       Construct mass matrix and solve K%iPHI%i%i = lambda*M%i * PHI%i%i",
										i + 1, i + 1, lcnum, lcnum,	i + 1, lcnum);
								Info("       use BC_%i.bin and RHS_%i_%i_%i.bin", i + 1, i + 1, j + 1, lcnum);
							}
							Info("\n");
						}
					} // Properties
				} // Responses
			} // nlc
		} // nbc
		for(unsigned int r = 0; r < GUIXml.Responses.size(); r++) {
			if ((GUIXml.Responses[r].Type == CENTER_OF_GRAVITY)
					| (GUIXml.Responses[r].Type == MASS)
					| (GUIXml.Responses[r].Type == VOLUME)
					| (GUIXml.Responses[r].Type == VOLUME_FRACTION) ){
				Info("Responses[%i], Type %s, Id %s", r,
						GUIXml.RespType2Str(GUIXml.Responses[r].Type),
						GUIXml.Responses[r].Id.c_str());
				continue;
			}
		}
	} // maxItr
}

//} /* namespace TO */
