#include "main.h"

typedef bool(__thiscall*InPredictionFn)(void*);

bool __fastcall new_InPrediction(void*thisptr,int edx)
{

	bool InPrediction = Prediction->GetMethod<InPredictionFn>(14)(thisptr);

	if ( (_ReturnAddress() == ((void*)CLIENT(0x69362))))
	{
		C_BasePlayer *LocalPlayer = (C_BasePlayer*)GetESI();

		QAngle *PunchAngleToRemove = *(QAngle**)(*(DWORD*)(GetEBP()) + 0xC);

		if ( !(LocalPlayer) || !(LocalPlayer->IsAlive()) )
		{
			return InPrediction;
		}
		else
		{
			PunchAngleToRemove->x -= LocalPlayer->GetPunchAngle().x;
			PunchAngleToRemove->y -= LocalPlayer->GetPunchAngle().y;
			PunchAngleToRemove->z -= LocalPlayer->GetPunchAngle().z;

			InPrediction = true;
		}
	}	

	return InPrediction;
}

const QAngle& C_BasePlayer::GetPunchAngle()
{
	return m_Local.m_vecPunchAngle.Get();
}