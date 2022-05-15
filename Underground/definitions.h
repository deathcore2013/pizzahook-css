#include "main.h"

#pragma once;

extern bool __fastcall new_InPrediction(void* ,int);
extern void __fastcall new_PaintTraverse(void*, int, int vguiPanel, bool forceRepaint, bool allowForce);
extern void *__fastcall new_FindMaterial(void*, int, const char *pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
extern CUserCmd* __fastcall new_GetUserCmd(void*, int, int sequence_number);
extern void __fastcall new_DrawPortals(void*,int);
extern void __fastcall new_DecodeUserCmdFromBuffer(void*, int, bf_read& buf, int sequence_number);
extern void __fastcall new_RunCommand(void*, int, C_BasePlayer*localplayer, CUserCmd*cmd, IMoveHelper*movehelper);
extern void __fastcall new_DrawModelExecute(void*,int,const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
extern bool __fastcall new_WriteUsercmdDeltaToBuffer(void*, int, bf_write *buf, int from, int to, bool isnewcommand);
extern void __fastcall new_FrameStageNotify(void*, int, ClientFrameStage_t curStage);
extern bool __fastcall new_CreateMoveClientMode(void*, int, float tick, CUserCmd*);
extern void __fastcall new_ExtraMouseSample(void*,int,float frametime, bool active);
extern void __fastcall new_CreateMoveClient(void*thisptr, int edx, int seq, float tick, bool active);