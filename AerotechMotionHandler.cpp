#include "AerotechMotionHandler.h"
#include <QtMessageHandler>

//******************************************
AerotechMotionHandler::AerotechMotionHandler() {
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
AerotechMotionHandler::~AerotechMotionHandler() {
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
bool AerotechMotionHandler::connectGantry(bool flag)
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
        if (A3200Disconnect(&gantry)) { //disconnect gantry here//CHECK &gantry or gantry?
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
bool AerotechMotionHandler::disconnectGantry()
{
    return connectGantry(false);
}

//------------------------------------------
bool MotionHandler::stop(){
    qInfo("stopping...");
    if (A3200MotionAbort(gantry, allAxes)) { //stop here
        qInfo("stop");
        return true;
    } else {
        qWarning("could not stop");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::acknowledgeMotionFaultGantry()
{
    qInfo("resetting errors...");
    //if (A3200MotionFaultAck(gantry, TASKID_Library, allAxes)) { //acknowledge and clear the fault on axes
    if (A3200AcknowledgeAll(gantry, TASKID_Library)) { //acknowledge all axis faults and clear all task errors
        qInfo("errors reset");
        return true;
    } else {
        qWarning("could not reset errors");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::enableAxes(bool flag)
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
bool AerotechMotionHandler::enableXAxis(bool flag)
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
bool AerotechMotionHandler::enableYAxis(bool flag)
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
bool AerotechMotionHandler::enableZAxis(bool flag)
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
bool AerotechMotionHandler::enableUAxis(bool flag)
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
bool AerotechMotionHandler::disableAxes()
{
    return enableAxes(false);
}

//------------------------------------------
bool AerotechMotionHandler::disableXAxis()
{
    return enableXAxis(false);
}

//------------------------------------------
bool AerotechMotionHandler::disableYAxis()
{
    return enableYAxis(false);
}

//------------------------------------------
bool AerotechMotionHandler::disableZAxis()
{
    return enableZAxis(false);
}

//------------------------------------------
bool AerotechMotionHandler::disableUAxis()
{
    return enableUAxis(false);
}

//******************************************
// home axes
//NOTE use QtConcurrent::run to run A3200MotionHome and allow position monitoring during the process

//------------------------------------------
bool AerotechMotionHandler::home() {
    qInfo("home axes");
    QtConcurrent::run(A3200MotionHome, gantry, TASKID_Library, allAxes); //home all axes here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::homeX() {
    qInfo("home x axis");
    QtConcurrent::run(A3200MotionHome, gantry, TASKID_Library, xAxis); //home x axis here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::homeY() {
    qInfo("home y axis");
    QtConcurrent::run(A3200MotionHome, gantry, TASKID_Library, yAxis); //home y axis here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::homeZ() {
    qInfo("home z axis");
    QtConcurrent::run(A3200MotionHome, gantry, TASKID_Library, zAxis); //home z axis here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::homeU() {
    qInfo("home u axis");
    QtConcurrent::run(A3200MotionHome, gantry, TASKID_Library, uAxis); //home u axis here
    return true;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool AerotechMotionHandler::moveTo(double x, double y, double z, double speed)
{
    qInfo("move to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...", x, y, z, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, xIndex, x, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, yIndex, y, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, zIndex, z, speed<40.?speed:40.); //move to destination here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveXTo(double x, double speed) {
    qInfo("move x axis to %.3f mm at %.3f mm/s speed", x, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, xIndex, x, speed); //move to destination here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveYTo(double y, double speed) {
    qInfo("move y axis to %.3f mm at %.3f mm/s speed", y, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, yIndex, y, speed); //move to destination here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveZTo(double z, double speed) {
    qInfo("move z axis to %.3f mm at %.3f mm/s speed", z, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, zIndex, z, speed); //move to destination here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveUTo(double u, double speed) {
    qInfo("move u axis to %.3f mm at %.3f mm/s speed", u, speed);
    A3200MotionMoveAbs(gantry, TASKID_Library, uIndex, u, speed); //move to destination here
    return true;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool AerotechMotionHandler::moveBy(double x, double y, double z, double speed)
{
    qInfo("move by (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed", x, y, z, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, xIndex, x, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, yIndex, y, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, zIndex, z, speed<40.?speed:40.); //move by step here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveXBy(double x, double speed) {
    qInfo("move x axis by %.3f mm at %.3f mm/s", x, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, xIndex, x, speed); //move by step here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveYBy(double y, double speed) {
    qInfo("move y axis by %.3f mm at %.3f mm/s", y, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, yIndex, y, speed); //move by step here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveZBy(double z, double speed) {
    qInfo("moving z axis by %.3f mm at %.3f mm/s", z, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, zIndex, z, speed); //move by step here
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::moveUBy(double u, double speed) {
    qInfo("move u axis by %.3f deg at %.3f deg/s", u, speed);
    A3200MotionMoveInc(gantry, TASKID_Library, uIndex, u, speed); //move by step here
    return true;
}


//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
bool AerotechMotionHandler::runX(double direction, double speed)
{
    qInfo("free running %sx axis at %.1f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, xIndex, direction<0?-1.*speed:speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along x axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::endRunX()
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
bool AerotechMotionHandler::runY(double direction, double speed)
{
    qInfo("free running %sy axis at %.1f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, yIndex, direction<0?-1.*speed:speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along y axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::endRunY()
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
bool AerotechMotionHandler::runZ(double direction, double speed)
{
    qInfo("free running %sz axis at %.1f mm/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, zIndex, direction<0?-1.*speed:speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along z axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::endRunZ()
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
bool AerotechMotionHandler::runU(double direction, double speed)
{
    qInfo("free running %su axis at %.1f deg/s", direction<0?"-":"+", speed);
    if (A3200MotionFreeRun(gantry, TASKID_Library, uIndex, direction<0?-1.*speed:speed)) { //free run here
        return true;
    } else {
        qWarning("could not free run along u axis");
        return false;
    }
    return true;
}

//------------------------------------------
bool AerotechMotionHandler::endRunU()
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
std::vector<double> AerotechMotionHandler::whereAmI() {
    std::vector<double> position = {qQNaN(), qQNaN(), qQNaN(), qQNaN()};
    if (gantryConnected) {
        double x, y, z, u;
        A3200StatusGetItem(gantry, xIndex, STATUSITEM_PositionFeedback, 0, &x);
        A3200StatusGetItem(gantry, yIndex, STATUSITEM_PositionFeedback, 0, &y);
        A3200StatusGetItem(gantry, zIndex, STATUSITEM_PositionFeedback, 0, &z);
        A3200StatusGetItem(gantry, uIndex, STATUSITEM_PositionFeedback, 0, &u);
        position = {x, y, z, u};
    }
    return position;
}
