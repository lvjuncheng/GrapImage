#ifndef _CPhaseInterFaster_H
#define _CPhaseInterFaster_H

#include "PhaseInterpolation.h"

struct XYZ_UVN
{
	int _x;
	int _y;
	int _UvN;
};

class CPhaseFast :public CPhaseInterpolation
{
public:
	CPhaseFast()
	{

	}

	virtual void PhaseRebuildFast_CPU();

protected:
private:
};

#endif 