#include "Gantry_Vancouver.h"

Gantry_Vancouver::Gantry_Vancouver()
{
	gantry = NULL;

	xaxis = (AXISMASK)AXISMASK_02;
	xindex = (AXISINDEX)AXISINDEX_02;

	yaxis = (AXISMASK)AXISMASK_00; // AXISMASK_01 is YY, the one coupled to Y without independent movement
	yindex = AXISINDEX_00;

	zaxis = (AXISMASK)AXISMASK_03;
	zindex = AXISINDEX_03;

	uaxis = (AXISMASK)AXISMASK_04;
	uindex = AXISINDEX_04;

	allAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00 | AXISMASK_03 | AXISMASK_04);
	xyAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00);

	default_xspeed = 20;
	default_yspeed = 20;
	default_zspeed = 5;
	default_uspeed = 5;
}


Gantry_Vancouver::~Gantry_Vancouver()
{
	if (gantry != NULL) {
        if (!A3200MotionDisable(gantry, TASKID_Library, allAxes)) qError("oh no");
        if (!A3200Disconnect(gantry)) qError("oh no");
	}
}

void Gantry_Vancouver::ActivateGantry()
{

	A3200Connect(&gantry);
    qInfo("Gantry activated!");
	return;
}

void Gantry_Vancouver::ActivateAllAxes()
{
	A3200MotionEnable(gantry, TASKID_Library, allAxes);
    qInfo("Axes activated");
	return;
}

void Gantry_Vancouver::Home() {

	A3200MotionHome(gantry, TASKID_Library, allAxes);
    qInfo("Homed all axes.");
	return;
}
