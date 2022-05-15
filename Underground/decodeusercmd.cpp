#include "main.h"

typedef void(__thiscall*DecodeUserCmdFromBuffer_t)(void*, bf_read& buf, int sequence_number);
void __fastcall new_DecodeUserCmdFromBuffer(void*thisptr,int edx,bf_read& buf, int sequence_number)
{
	Input->GetMethod<DecodeUserCmdFromBuffer_t>(7)(thisptr,buf, sequence_number);

	CUserCmd *cmd = &(*(CUserCmd**)((DWORD)InputSDK + 0xC4))[sequence_number % 90];
	C_BasePlayer* LocalPlayer = (C_BasePlayer*)EntityListSDK->GetClientEntity(EngineSDK->GetLocalPlayer());

	MovementRecorder.RecordMovement(cmd, LocalPlayer);
}