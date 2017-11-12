#pragma once
#include "MotionHandler.h"
#include "A3200.h"
#include <iostream>
#include <tchar.h>
#include <math.h>

class Gantry_Vancouver :
	public MotionHandler
{
public:

	Gantry_Vancouver();
	~Gantry_Vancouver();

	// Turn the gantry on
	void ActivateGantry();

	// Need to activate axes before machine can be used
	void ActivateAllAxes();

	// Home all axes
	void Home();

private :

	// Handle to give access to A3200
	A3200Handle gantry;

	AXISMASK xaxis;
	AXISINDEX xindex;

	AXISMASK yaxis;
	AXISINDEX yindex;

	AXISMASK zaxis;
	AXISINDEX zindex;

	AXISMASK uaxis;
	AXISINDEX uindex;

	AXISMASK allAxes;
	AXISMASK xyAxes;

	double default_xspeed;
	double default_yspeed;
	double default_zspeed;
	double default_uspeed;
};
