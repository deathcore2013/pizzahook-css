#ifndef _CONTROL_VARIABLES_H_
#define _CONTROL_VARIABLES_H_
//===================================================================================
class CControlVariables
{
public:

	CControlVariables();

	void Initialize(void);

	//Add vars here.
	float enableaim;
	float silentaim;

	float name;
	float weapon;

	float perfectaccuracy;
	float forcelowspread;

	float roundsay;
	float antiaim3;
	float antiaim;
	float fakelag;
	float fakelag2;
	float fakelaginattack2;
	float fakelaginattack;
	float antiaim2;
	float perfectsilentaim;

	float red;
	float green;
	float blue;
	float xpos;
	float ypos;
	float weight;
	float alpha;
};
//===================================================================================
extern CControlVariables UndergroundVars;
//===================================================================================
#endif