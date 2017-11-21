#include "MotionHandler.h"
#include <QtMessageHandler>

//******************************************
MotionHandler::MotionHandler() :
    gantryConnected(false),
    xAxisEnabled(false),
    yAxisEnabled(false),
    zAxisEnabled(false),
    uAxisEnabled(false)
    {
    gantry = NULL;

    xAxis = (AXISMASK)AXISMASK_02;
    xIndex = (AXISINDEX)AXISINDEX_02;

    yAxis = (AXISMASK)AXISMASK_00; // AXISMASK_01 is YY, the y-axis slave
    yIndex = AXISINDEX_00;

    zAxis = (AXISMASK)AXISMASK_03;
    zIndex = AXISINDEX_03;

    uAxis = (AXISMASK)AXISMASK_04;
    uIndex = AXISINDEX_04;

    allAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00 | AXISMASK_03 | AXISMASK_04);
    xyAxes = (AXISMASK)(AXISMASK_02 | AXISMASK_00);
}

//******************************************
MotionHandler::~MotionHandler() {
    if (gantry != NULL) {
        if (!A3200MotionDisable(gantry, TASKID_Library, allAxes))
            qWarning("axes disabled");
        if (!A3200Disconnect(gantry))
            qWarning("gantry disconnected");
    }
}

//******************************************
// connect to the gantry

//------------------------------------------
bool MotionHandler::connectGantry(bool flag)
{
    if (flag) {
        qInfo("connecting gantry...");
        if (A3200Connect(&gantry)) { //connect gantry here
            qInfo("gantry connected");
            gantryConnected=true;
            return true;
        } else {
            qWarning("could not connect gantry");
            return false;
        }
    } else {
        qInfo("disconnecting gantry...");
        if (A3200Disconnect(&gantry)) { //disconnect gantry here
            qInfo("gantry disconnected");
            gantryConnected=false;
            return true;
        }else {
            qWarning("could not disconnect gantry");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::disconnectGantry()
{
    return connectGantry(false);
}

//------------------------------------------
bool MotionHandler::enableAxes(bool flag)
{
    if (flag) {
        qInfo("enabling axes...");
        if (A3200MotionEnable(gantry, TASKID_Library, allAxes)) { //enable all axes here
            qInfo("axes enabled");
            return true;
        } else {
            qWarning("could not enable axes");
            return false;
        }
    } else {
        qInfo("disabling axes...");
        if (A3200MotionDisable(gantry, TASKID_Library, allAxes)) { //disable all axes here
            qInfo("axes disabled");
            return true;
        } else {
            qWarning("could not disable axes");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::enableXAxis(bool flag)
{
    if (flag) {
        qInfo("enabling x axis...");
        if (A3200MotionEnable(gantry, TASKID_Library, xAxis)) { //enable x axis here
            qInfo("x axis enabled");
            xAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable x axis");
            return false;
        }
    } else {
        qInfo("disabling x axis...");
        if (A3200MotionDisable(gantry, TASKID_Library, xAxis)) { //disable x axis here
            qInfo("x axis disabled");
            xAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable x axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::enableYAxis(bool flag)
{
    if (flag) {
        qInfo("enabling y axis...");
        if (A3200MotionEnable(gantry, TASKID_Library, yAxis)) { //enable y axis here
            qInfo("y axis enabled");
            yAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable y axis");
            return false;
        }
    } else {
        qInfo("disabling y axis...");
        if (A3200MotionDisable(gantry, TASKID_Library, yAxis)) { //disable y axis here
            qInfo("y axis disabled");
            yAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable y axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::enableZAxis(bool flag)
{
    if (flag) {
        qInfo("enabling z axis...");
        if (A3200MotionEnable(gantry, TASKID_Library, zAxis)) { //enable z axis here
            qInfo("z axis enabled");
            zAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable z axis");
            return false;
        }
    } else {
        qInfo("disabling z axis...");
        if (A3200MotionDisable(gantry, TASKID_Library, zAxis)) { //disable z axis here
            qInfo("z axis disabled");
            zAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable z axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::enableUAxis(bool flag)
{
    if (flag) {
        qInfo("enabling u axis...");
        if (A3200MotionEnable(gantry, TASKID_Library, uAxis)) { //enable u axis here
            qInfo("u axis enabled");
            uAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable u axis");
            return false;
        }
    } else {
        qInfo("disabling u axis...");
        if (A3200MotionDisable(gantry, TASKID_Library, uAxis)) { //disable u axis here
            qInfo("u axis disabled");
            uAxisEnabled=false;
            return true;
        } else {
            qInfo("could not disable u axis");
            return false;
        }
    }
    return true;
}

//------------------------------------------
bool MotionHandler::disableAxes()
{
    return enableAxes(false);
}

//------------------------------------------
bool MotionHandler::disableXAxis()
{
    return enableXAxis(false);
}

//------------------------------------------
bool MotionHandler::disableYAxis()
{
    return enableYAxis(false);
}

//------------------------------------------
bool MotionHandler::disableZAxis()
{
    return enableZAxis(false);
}

//------------------------------------------
bool MotionHandler::disableUAxis()
{
    return enableUAxis(false);
}

//******************************************
// home axes

//------------------------------------------
bool MotionHandler::home() {
    qInfo("homing axes...");
    if (A3200MotionHome(gantry, TASKID_Library, allAxes)) { //home all axes here
        qInfo("axes homed");
        return true;
    } else {
        qWarning("could not home axes");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::homeX() {
    qInfo("homing x axis...");
    if (A3200MotionHome(gantry, TASKID_Library, xAxis)) { //home x axis here
        qInfo("x axis homed");
        return true;
    } else {
        qWarning("could not home x axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::homeY() {
    qInfo("homing y axis...");
    if (A3200MotionHome(gantry, TASKID_Library, yAxis)) { //home y axis here
        qInfo("y axis homed");
        return true;
    } else {
        qWarning("could not home y axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::homeZ() {
    qInfo("homing z axis...");
    if (A3200MotionHome(gantry, TASKID_Library, zAxis)) { //home z axis here
        qInfo("z axis homed");
        return true;
    } else {
        qWarning("could not home z axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::homeU() {
    qInfo("homing u axis...");
    if (A3200MotionHome(gantry, TASKID_Library, uAxis)) { //home u axis here
        qInfo("u axis homed");
        return true;
    } else {
        qWarning("could not home u axis");
        return false;
    }
    return true;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool MotionHandler::moveTo(double x, double y, double z, double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...", x, y, z, speed);
    if (A3200MotionMoveAbs(gantry, TASKID_Library, xIndex, x, speed) &&
        A3200MotionMoveAbs(gantry, TASKID_Library, yIndex, y, speed) &&
        A3200MotionMoveAbs(gantry, TASKID_Library, zIndex, z, speed/4.)) { //move to destination (and wait) here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved to destination");
        return true;
    } else {
        qWarning("could not move to destination");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveXTo(double x, double speed) {
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    if (A3200MotionMoveAbs(gantry, TASKID_Library, xIndex, x, speed)) { //move to destination here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved x axis to destination");
        return true;
    } else {
        qWarning("could not move x axis to destination");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveYTo(double y, double speed) {
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if (A3200MotionMoveAbs(gantry, TASKID_Library, yIndex, y, speed)) { //move to destination here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved y axis to destination");
        return true;
    } else {
        qWarning("could not move y axis to destination");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveZTo(double z, double speed) {
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if (A3200MotionMoveAbs(gantry, TASKID_Library, zIndex, z, speed)) { //move to destination here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved z axis to destination");
        return true;
    } else {
        qWarning("could not move z axis to destination");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveUTo(double u, double speed) {
    qInfo("moving u axis to %.3f mm at %.3f mm/s speed", u, speed);
    if (A3200MotionMoveAbs(gantry, TASKID_Library, uIndex, u, speed)) { //move to destination here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved u axis to destination");
        return true;
    } else {
        qWarning("could not move u axis to destination");
        return false;
    }
    return true;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool MotionHandler::moveBy(double x, double y, double z, double speed)
{
    qInfo("moving by (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed", x, y, z, speed);
    if (A3200MotionMoveInc(gantry, TASKID_Library, xIndex, x, speed) &&
        A3200MotionMoveInc(gantry, TASKID_Library, yIndex, y, speed) &&
        A3200MotionMoveInc(gantry, TASKID_Library, zIndex, z, speed/4.)) { //move by step here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved by step");
        return true;
    } else {
        qWarning("could not move by step");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveXBy(double x, double speed) {
    qInfo("moving x axis by %.3f mm at %.3f mm/s", x, speed);
    if (A3200MotionMoveInc(gantry, TASKID_Library, xIndex, x, speed)) { //move by step here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved x axis by step");
        return true;
    } else {
        qWarning("could not move x axis by step");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveYBy(double y, double speed) {
    qInfo("moving y axis by %.3f mm at %.3f mm/s", y, speed);
    if (A3200MotionMoveInc(gantry, TASKID_Library, yIndex, y, speed)) { //move by step here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved y axis by step");
        return true;
    } else {
        qWarning("could not move y axis by step");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveZBy(double z, double speed) {
    qInfo("moving z axis by %.3f mm at %.3f mm/s", z, speed);
    if (A3200MotionMoveInc(gantry, TASKID_Library, zIndex, z, speed)) { //move by step here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved z axis by step");
        return true;
    } else {
        qWarning("could not move z axis by step");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::moveUBy(double u, double speed) {
    qInfo("moving u axis by %.3f deg at %.3f deg/s", u, speed);
    if (A3200MotionMoveInc(gantry, TASKID_Library, uIndex, u, speed)) { //move by step here
        A3200MotionWaitForMotionDone(gantry, allAxes, WAITOPTION_InPosition, -1, NULL); //wait
        qInfo("moved u axis by step");
        return true;
    } else {
        qWarning("could not move u axis by step");
        return false;
    }
    return true;
}


//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool MotionHandler::runX(double direction, double speed)
{
    qInfo("free running %sx axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, xIndex, speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along x axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::endRunX()
{
    qInfo("stop free running along x axis");
    if (A3200MotionFreeRunStop(gantry, TASKID_Library, xIndex)) { //stop free run here
        return true;
    } else {
        qWarning("could not stop free run along x axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::runY(double direction, double speed)
{
    qInfo("free running %sy axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, yIndex, speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along y axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::endRunY()
{
    qInfo("stop free running along y axis");
    if (A3200MotionFreeRunStop(gantry, TASKID_Library, yIndex)) { //stop free run here
        return true;
    } else {
        qWarning("could not stop free run along y axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::runZ(double direction, double speed)
{
    qInfo("free running %sz axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, zIndex, speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along z axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::endRunZ()
{
    qInfo("stop free running along z axis");
    if (A3200MotionFreeRunStop(gantry, TASKID_Library, zIndex)) { //stop free run here
        return true;
    } else {
        qWarning("could not stop free run along z axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::runU(double direction, double speed)
{
    qInfo("free running %su axis at %.3f deg/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, uIndex, speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along u axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool MotionHandler::endRunU()
{
    qInfo("stop free running along u axis");
    if (A3200MotionFreeRunStop(gantry, TASKID_Library, uIndex)) { //stop free run here
        return true;
    } else {
        qWarning("could not stop free run along u axis");
        return false;
    }
    return true;
}

//******************************************
//gantry current position
std::vector<double> MotionHandler::whereAmI() {
    std::vector<double> position = {0.0,0.0,0.0,0.0};
    return position;
}
