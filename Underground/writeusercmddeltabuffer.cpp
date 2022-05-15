#include "definitions.h"

#define WEAPON_SUBTYPE_BITS	6

void WriteUsercmd(bf_write *buf, CUserCmd *to)
{

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->command_number, 32);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->tick_count, 32);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->viewangles[0]);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->viewangles[1]);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->viewangles[2]);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->forwardmove);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->sidemove);

	buf->WriteOneBit(1);
	buf->WriteFloat(to->upmove);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->buttons, 32);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->impulse, 8);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->weaponselect, MAX_EDICT_BITS);

	buf->WriteOneBit(1);
	buf->WriteUBitLong(to->weaponsubtype, WEAPON_SUBTYPE_BITS);

	buf->WriteOneBit(1);
	buf->WriteShort(to->mousedx);

	buf->WriteOneBit(1);
	buf->WriteShort(to->mousedy);

}

bool __fastcall new_WriteUsercmdDeltaToBuffer(void*thisptr,int edx,bf_write *buf, int from, int to, bool isnewcommand)
{
	CUserCmd *Cmd = &((CInput*)thisptr)->m_pCommands[to % MULTIPLAYER_BACKUP];

	WriteUsercmd(buf,Cmd);

	return true;
}