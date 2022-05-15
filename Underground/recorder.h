#include "main.h"

#define MaxRecordTicks 10000000


#ifndef RECORDER
#define RECORDER

class CRecord
{
public:

	QAngle	viewangles;     
	float	forwardmove;   
	float	sidemove;      
	float	upmove;         
	int		buttons;		      
	int		weaponselect;	
	int		weaponsubtype;

	CRecord()
	{
		viewangles.Init();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		weaponselect = 0;
		weaponsubtype = 0;
	}

	void Reset()
	{
		viewangles.Init();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		weaponselect = 0;
		weaponsubtype = 0;
	}

};

struct SRecord
{
	int buttons;
	float forward;
	float side;
	float up;
	QAngle view;
};

class CMovementRecorder
{
public:
	CMovementRecorder(void);

	void RecordMovement(CUserCmd*Cmd,C_BasePlayer*LocalPlayer);
	CRecord *Movements;
	SRecord *TempCmd;
	int Ticks;
	int Situation;
	Vector *DrawPath;
	int TickEnd;
	bool AutoPlayer;
	int FindStart;
	void SmootherAngles(C_BasePlayer* LocalPlayer);

}extern MovementRecorder;

enum
{
	NOTHING = 0,
	RECORDING = 1,
	PLAYING = 2,
	STARTPOS = 3

};

#endif