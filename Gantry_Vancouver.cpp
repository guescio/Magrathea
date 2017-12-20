#include "Gantry_Vancouver.h"

#include <sys/stat.h>
#include <fstream>
#include <QDebug>

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

    default_xspeed = 75;
    default_yspeed = 75;
    default_zspeed = 25;
    default_uspeed = 50;

}


Gantry_Vancouver::~Gantry_Vancouver()
{
    if (gantry != NULL) {
        if (!A3200MotionDisable(gantry, TASKID_Library, allAxes)) qInfo("oh no: did not disable motion correctly");
        if (!A3200Disconnect(gantry)) qInfo("oh no: did not disconnect gantry correctly");
	}
}

void Gantry_Vancouver::ActivateGantry()
{
	A3200Connect(&gantry);
    qInfo("activate gantry");


    // Set up ramping
    A3200MotionSetupRampModeAxis(gantry, TASKID_01, allAxes, RAMPMODE_Time);
    A3200MotionSetupRampRateCoordinated(gantry, TASKID_01, 5);
	return;
}

void Gantry_Vancouver::ActivateAllAxes()
{
	A3200MotionEnable(gantry, TASKID_Library, allAxes);
    qInfo("enable axes");
	return;
}

void Gantry_Vancouver::Home() {
	A3200MotionHome(gantry, TASKID_Library, allAxes);
    qInfo("home axes");
    return;
}

void Gantry_Vancouver::ConnectJoystick() {

    qInfo("Connecting joystick");
    if(!A3200ProgramRun(gantry, TASKID_01, "C:\\Users\\canuck\\Desktop\\Magrathea\\Magrathea\\AeroBasicScripts\\connect_joystick.pgm")) {
        qInfo("Failed to connect joystick!");
        CHAR data[1024];
        A3200GetLastErrorString(data, 1024);
        qInfo() << "Error: " <<  data ;
    }
    return;
}

void Gantry_Vancouver::DisconnectJoystick() {

    qInfo("returning motion control to GUI");
    A3200ProgramStopAndWait(gantry, TASKID_01, 5000);
    return;

}

void Gantry_Vancouver::HomeX() {

    A3200MotionHome(gantry, TASKID_Library, xaxis);
    qInfo("Homed X axis.");
    return;
}

void Gantry_Vancouver::HomeY() {

    A3200MotionHome(gantry, TASKID_Library, yaxis);
    qInfo("Homed Y axis.");
    return;
}
void Gantry_Vancouver::HomeZ() {

    A3200MotionHome(gantry, TASKID_Library, zaxis);
    qInfo("Homed Z axis.");
    return;
}
void Gantry_Vancouver::HomeU() {

    A3200MotionHome(gantry, TASKID_Library, uaxis);
    qInfo("Homed U axis.");
    return;
}

void Gantry_Vancouver::MoveTo(double x, double y, double z, double u, double speed)
{
    if (!(isnan(speed))) {
        std::cout << "hello i am here" << std::endl;
        A3200MotionMoveAbs(gantry, TASKID_Library, xindex, x, speed);
        A3200MotionMoveAbs(gantry, TASKID_Library, yindex, y, speed);
        A3200MotionMoveAbs(gantry, TASKID_Library, zindex, z, speed / 4.0);
    }
    else {
        A3200MotionMoveAbs(gantry, TASKID_Library, xindex, x, default_xspeed);
        A3200MotionMoveAbs(gantry, TASKID_Library, yindex, y, default_yspeed);
        A3200MotionMoveAbs(gantry, TASKID_Library, zindex, z, default_zspeed);
    }
    A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL);

    return;
}

void Gantry_Vancouver::MoveZTo(double z, double speed) {
    if (!(isnan(speed))) {
        A3200MotionMoveAbs(gantry, TASKID_Library, zindex, z, speed);
    }
    else {
        A3200MotionMoveAbs(gantry, TASKID_Library, zindex, z, default_zspeed);
    }
    A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL);
    return;
}

// Move relative to current location by an amount x, y, z
// Note default unit is mm
void Gantry_Vancouver::MoveBy(double x, double y, double z, double u, double speed)
{

    if (!(isnan(speed))) {
        A3200MotionMoveInc(gantry, TASKID_Library, xindex, x, speed);
        A3200MotionMoveInc(gantry, TASKID_Library, yindex, y, speed);
        A3200MotionMoveInc(gantry, TASKID_Library, zindex, z, speed / 4.0);
    }
    else {
        A3200MotionMoveInc(gantry, TASKID_Library, xindex, x, default_xspeed);
        A3200MotionMoveInc(gantry, TASKID_Library, yindex, y, default_yspeed);
        A3200MotionMoveInc(gantry, TASKID_Library, zindex, z, default_zspeed);
    }
    A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL);

    return;
}

void Gantry_Vancouver::MoveZBy(double z, double speed) {
    if (!(isnan(speed))) {
        A3200MotionMoveInc(gantry, TASKID_Library, zindex, z, speed);
    }
    else {
        A3200MotionMoveInc(gantry, TASKID_Library, zindex, z, default_zspeed);
    }
    A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL);
    return;
}

// Begin free running (x direction)
void Gantry_Vancouver::RunX()
{
    qInfo("Free running in X...");
    A3200MotionFreeRun(gantry, TASKID_Library, xindex, default_xspeed);
    return;
}

// Begin free running (negative x direction)
void Gantry_Vancouver::RunXNeg()
{
    qInfo("Free running in negative X...");
    A3200MotionFreeRun(gantry, TASKID_Library, xindex, -1.0*default_xspeed);
    return;
}

// Stop free running (x direction)
void Gantry_Vancouver::EndRunX()
{
    A3200MotionFreeRunStop(gantry, TASKID_Library, xindex);
    qInfo("Stopped free running in X");
    return;
}

// Begin free running (y direction)
void Gantry_Vancouver::RunY()
{
    qInfo("Free running in Y...");
    A3200MotionFreeRun(gantry, TASKID_Library, yindex, default_yspeed);
    return;
}

// Begin free running (negative y direction)
void Gantry_Vancouver::RunYNeg()
{
    qInfo("Free running in negative Y...");
    A3200MotionFreeRun(gantry, TASKID_Library, yindex, -1.0*default_yspeed);
    return;
}

// Stop free running (y direction)
void Gantry_Vancouver::EndRunY()
{
    A3200MotionFreeRunStop(gantry, TASKID_Library, yindex);
    qInfo("Stopped free running in Y");
    return;
}

// Begin free running (z direction)
void Gantry_Vancouver::RunZ()
{
    A3200MotionFreeRun(gantry, TASKID_Library, zindex, default_zspeed);
    qInfo("Free running in Z...");
    return;
}

// Begin free running (negative z direction)
void Gantry_Vancouver::RunZNeg()
{
    A3200MotionFreeRun(gantry, TASKID_Library, zindex, -1.0*default_zspeed);
    qInfo("Free running in negative Z...");
    return;
}

// Stop free running (z direction)
void Gantry_Vancouver::EndRunZ()
{
    A3200MotionFreeRunStop(gantry, TASKID_Library, zindex);
    qInfo("Stopped free running in Z");
    return;
}

// Begin free running (rotational direction)
void Gantry_Vancouver::RunU()
{
    A3200MotionFreeRun(gantry, TASKID_Library, uindex, default_uspeed);
    qInfo("Free running rotationally...");
    return;
}

// Begin free running (negative u direction)
void Gantry_Vancouver::RunUNeg()
{
    A3200MotionFreeRun(gantry, TASKID_Library, uindex, -1.0*default_uspeed);
    qInfo("Free running rotationally (backwards) ...");
    return;
}

// Stop free running (rotational direction)
void Gantry_Vancouver::EndRunU()
{
    A3200MotionFreeRunStop(gantry, TASKID_Library, uindex);
    qInfo("Stopped free running rotationally");
    return;
}

void Gantry_Vancouver::SetSpeedDefaults(double speedX, double speedY, double speedZ, double speedU)
{
    qInfo() << "Setting speed defaults to:";
    qInfo() << "\nx axis:" << speedX;
    qInfo() << "\ny axis:" << speedY;
    qInfo() << "\nz axis:" << speedZ;
    qInfo() << "\nrotational axis:" << speedU;

    default_xspeed = speedX;
    default_yspeed = speedY;
    default_zspeed = speedZ;
    default_uspeed = speedU;

    return;
}

void Gantry_Vancouver::SetSpeedX(double speed)
{
    qInfo() << "Setting x axis speed to" << speed;
    default_xspeed = speed;
    return;
}

void Gantry_Vancouver::SetSpeedY(double speed)
{
    qInfo() << "Setting y axis speed to" << speed;
    default_yspeed = speed;
    return;
}

void Gantry_Vancouver::SetSpeedZ(double speed)
{
    qInfo() << "Setting z axis speed to" << speed;
    default_zspeed = speed;
    return;
}

void Gantry_Vancouver::SetSpeedU(double speed)
{
    qInfo() << "Setting rotational speed to" << speed;
    default_uspeed = speed;
    return;
}

std::vector<double> Gantry_Vancouver::WhereAmI() {
    double posX, posY, posZ, posU;
    A3200StatusGetItem(gantry, xindex, STATUSITEM_PositionFeedback, 0, &posX);
    A3200StatusGetItem(gantry, yindex, STATUSITEM_PositionFeedback, 0, &posY);
    A3200StatusGetItem(gantry, zindex, STATUSITEM_PositionFeedback, 0, &posZ);
    A3200StatusGetItem(gantry, uindex, STATUSITEM_PositionFeedback, 0, &posU);
    std::vector<double> returnVec = {posX, posY, posZ, posU};
    qInfo() << "Gantry is at position" << posX << posY << posZ << posU;

    return returnVec;
}
