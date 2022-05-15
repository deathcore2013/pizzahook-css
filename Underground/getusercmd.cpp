#include "main.h"

CUserCmd* __fastcall new_GetUserCmd(void*thisptr, int edx, int sequence_number)
{
	return  &(*(CUserCmd**)((DWORD)InputSDK + 0xC4))[ sequence_number % 90 ];
}