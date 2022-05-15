#include "macro.h"

//credits to backwards
CMacro Macro;


void CMacro::SaveMacro(CRecord* movement, Vector*DrawPath, int &TickEnd, const char* filename)
{
	FILE* fp = fopen(filename, "wb+");
	if (fp != NULL)
	{
		fwrite(&TickEnd, sizeof(int), 1, fp);
		fwrite(DrawPath, sizeof(Vector), TickEnd--, fp);
		fwrite(movement, sizeof(CRecord), TickEnd--, fp);
		fclose(fp);
	}
	else
	{
		Console.Print("Failed to save macro");
	}
}

void CMacro::ReadMacro(CRecord* movement, Vector*DrawPath, int &TickEnd, const char* filename)
{
	FILE* fp = fopen(filename, "rb+");
	if (fp != NULL)
	{
		fread(&TickEnd, sizeof(int), 1, fp);
		fread(DrawPath, sizeof(Vector), TickEnd--, fp);
		fread(movement, sizeof(CRecord), TickEnd--, fp);
		fclose(fp);
	}
	else
	{
		Console.Print("Failed to load macro maybe wrong filename?");
	}
}

void CMacro::ReadMacro2(SRecord* movement, int &tick_count, Vector &StartPosition, const char* filename)
{
	FILE* fp = fopen(filename, "rb+");
	if (fp != NULL)
	{
		fread(&tick_count, sizeof(int), 1, fp);//Header Tick_count
		fread(&StartPosition, sizeof(Vector), 1, fp);//Header Start Position
		fread(movement, sizeof(SRecord), tick_count--, fp);
		fclose(fp);
	}
	else
		Console.Print("Failed to load macro maybe wrong filename?");
}