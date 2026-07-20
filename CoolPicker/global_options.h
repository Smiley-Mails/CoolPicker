#ifndef _Global_Options_
#define _Global_Options_

#include "global_constants.h"

class GlobalOptions
{
public:
	BOOL FloatingPointMode; //New variable, not really needed, will take it out eventually
	int planeSubmode;
	int bigMode;
	int rangeType;
	int rangeMax;
	float rangeCoeff;
	int flow;
	int mixMode;
	DWORD mixBack;
	AColor a_mixBack;

	GlobalOptions(): //Constructor
		FloatingPointMode(FALSE),
		planeSubmode(MODE_HS),
		bigMode(MODE_BIGPLANE),
		rangeType(255),
		rangeMax(255),
		rangeCoeff(1.0f),
		mixBack(0),
		flow(30),
		mixMode(1)
	{ 
		//Add anything here I want
	}
	~GlobalOptions() {}
};

#endif