#include "main.h"

#ifndef PREDICTION
#define PREDICTION

class CMyPrediction
{
public:
	CMyPrediction();

	void RunLocalPrediction(CUserCmd*Cmd,C_BasePlayer*LocalPlayer);
	void FinishLocalPrediction(CUserCmd*Cmd,C_BasePlayer*LocalPlayer);
	void RunSimulation(CUserCmd*Cmd, C_BasePlayer*LocalPlayer);
	void RunEntsPrediction(CUserCmd*Cmd, C_BasePlayer*LocalPlayer);

	CMoveData MoveData;
	IMoveHelper *Helper;
	float oldcurtime,oldframetime;
	float Difference[0xFF];
	CUserCmd*OldCmd;
	C_BasePlayer*OldPlayer;
	CUserCmd cmd;
	float x, y;

}extern MyPrediction;

#endif