#include "MotionHandler.h"
#include <QtMessageHandler>
//#include <unistd.h>//TEST usleep function
#include <QCoreApplication>//TEST
#include <QFuture>//TEST
#include <QtConcurrent>//TEST

//******************************************
MotionHandler::MotionHandler() :
    gantryConnected(false),
    xAxisEnabled(false),
    yAxisEnabled(false),
    zAxisEnabled(false),
    uAxisEnabled(false)
    {}

//******************************************
MotionHandler::~MotionHandler() {}


//******************************************
//TEST
void MotionHandler::motionTest() {
    qInfo("running test...");
    //fprintf(stderr, "TEST running test");
    //usleep(5e6);
    //QFuture<void> t = QtConcurrent::run(usleep, 5e6);
    //t.waitForFinished();
    //QtConcurrent::run(motionInception);
    qInfo("test run");
    //fprintf(stderr, "TEST test run");
    return;
}

void MotionHandler::motionInception() {
    //qInfo("going deeper...");//NOTE using qInfo gives problems
    fprintf(stderr, "TEST: going deeper...");
    //usleep(5e6);
    //qInfo("done, now get out");//NOTE using qInfo gives problems
    fprintf(stderr, "TEST: done, now get out");
    return;
}
//END TEST
//******************************************

//******************************************
// connect to the gantry

//------------------------------------------
bool MotionHandler::connectGantry(bool flag)
{
    if (flag) {
        qInfo("connecting gantry...");
        if (true) { //connect gantry here
            qInfo("gantry connected");
            gantryConnected=true;
            return true;
        } else {
            qWarning("could not connect gantry");
            return false;
        }
    } else {
        qInfo("disconnecting gantry...");
        if (true) { //disconnect gantry here
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
void MotionHandler::acknowledgeMotionFaultGantry()
{
    qInfo("resetting errors...");
    if (true) { //acknowledge and clear axes faults
        qInfo("errors reset");
    } else {
        qWarning("could not reset errors");
    }
    return;
}

//------------------------------------------
bool MotionHandler::enableAxes(bool flag)
{
    if (flag) {
        qInfo("enabling axes...");
        if (true) { //enable all axes here
            qInfo("axes enabled");
            return true;
        } else {
            qWarning("could not enable axes");
            return false;
        }
    } else {
        qInfo("disabling axes...");
        if (true) { //disable all axes here
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
        if (true) { //enable x axis here
            qInfo("x axis enabled");
            xAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable x axis");
            return false;
        }
    } else {
        qInfo("disabling x axis...");
        if (true) { //disable x axis here
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
        if (true) { //enable y axis here
            qInfo("y axis enabled");
            yAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable y axis");
            return false;
        }
    } else {
        qInfo("disabling y axis...");
        if (true) { //disable y axis here
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
        if (true) { //enable z axis here
            qInfo("z axis enabled");
            zAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable z axis");
            return false;
        }
    } else {
        qInfo("disabling z axis...");
        if (true) { //disable z axis here
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
        if (true) { //enable u axis here
            qInfo("u axis enabled");
            uAxisEnabled=true;
            return true;
        } else {
            qInfo("could not enable u axis");
            return false;
        }
    } else {
        qInfo("disabling u axis...");
        if (true) { //disable u axis here
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
void MotionHandler::home() {
    qInfo("homing axes...");
    if (true) { //home all axes here
        qInfo("axes homed");
    } else {
        qWarning("could not home axes");
    }
    return;
}

//------------------------------------------
void MotionHandler::homeX() {
    qInfo("homing x axis...");
    if (true) { //home x axis here
        qInfo("x axis homed");
    } else {
        qWarning("could not home x axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::homeY() {
    qInfo("homing y axis...");
    if (true) { //home y axis here
        qInfo("y axis homed");
    } else {
        qWarning("could not home y axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::homeZ() {
    qInfo("homing z axis...");
    if (true) { //home z axis here
        qInfo("z axis homed");
    } else {
        qWarning("could not home z axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::homeU() {
    qInfo("homing u axis...");
    if (true) { //home u axis here
        qInfo("u axis homed");
    } else {
        qWarning("could not home u axis");
    }
    return;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::moveTo(double x, double y, double z, double speed)
{
    qInfo("moving to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed...", x, y, z, speed);
    if (true) { //move to destination here
        qInfo("moved to destination");
    } else {
        qWarning("could not move to destination");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveXTo(double x, double speed) {
    qInfo("moving x axis to %.3f mm at %.3f mm/s speed", x, speed);
    if (true) { //move to destination here
        qInfo("moved x axis to destination");
    } else {
        qWarning("could not move x axis to destination");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveYTo(double y, double speed) {
    qInfo("moving y axis to %.3f mm at %.3f mm/s speed", y, speed);
    if (true) { //move to destination here
        qInfo("moved y axis to destination");
    } else {
        qWarning("could not move y axis to destination");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveZTo(double z, double speed) {
    qInfo("moving z axis to %.3f mm at %.3f mm/s speed", z, speed);
    if (true) { //move to destination here
        qInfo("moved z axis to destination");
    } else {
        qWarning("could not move z axis to destination");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveUTo(double u, double speed) {
    qInfo("moving u axis to %.3f mm at %.3f mm/s speed", u, speed);
    if (true) { //move to destination here
        qInfo("moved u axis to destination");
    } else {
        qWarning("could not move u axis to destination");
    }
    return;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::moveBy(double x, double y, double z, double speed)
{
    qInfo("moving by (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed", x, y, z, speed);
    if (true) { //move by step here
        qInfo("moved by step");
    } else {
        qWarning("could not move by step");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveXBy(double x, double speed) {
    qInfo("moving x axis by %.3f mm at %.3f mm/s", x, speed);
    if (true) { //move by step here
        qInfo("moved x axis by step");
    } else {
        qWarning("could not move x axis by step");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveYBy(double y, double speed) {
    qInfo("moving y axis by %.3f mm at %.3f mm/s", y, speed);
    if (true) { //move by step here
        qInfo("moved y axis by step");
    } else {
        qWarning("could not move y axis by step");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveZBy(double z, double speed) {
    qInfo("moving z axis by %.3f mm at %.3f mm/s", z, speed);
    if (true) { //move by step here
        qInfo("moved z axis by step");
    } else {
        qWarning("could not move z axis by step");
    }
    return;
}

//------------------------------------------
void MotionHandler::moveUBy(double u, double speed) {
    qInfo("moving u axis by %.3f deg at %.3f deg/s", u, speed);
    if (true) { //move by step here
        qInfo("moved u- axis by step");
    } else {
        qWarning("could not move u axis by step");
    }
    return;
}


//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::runX(double direction, double speed)
{
    qInfo("free running %sx axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (!true) { //free run here
        qWarning("could not free run along x axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::endRunX()
{
    qInfo("stop free running along x axis");
    if (!true) { //stop free run here
        qWarning("could not stop free run along x axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::runY(double direction, double speed)
{
    qInfo("free running %sy axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (!true) { //free run here
        qWarning("could not free run along y axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::endRunY()
{
    qInfo("stop free running along y axis");
    if (!true) { //stop free run here
        qWarning("could not stop free run along y axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::runZ(double direction, double speed)
{
    qInfo("free running %sz axis at %.3f mm/s", direction<0?"-":"+", speed);
    if (!true) { //free run here
        qWarning("could not free run along z axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::endRunZ()
{
    qInfo("stop free running along z axis");
    if (!true) { //stop free run here
        qWarning("could not stop free run along z axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::runU(double direction, double speed)
{
    qInfo("free running %su axis at %.3f deg/s", direction<0?"-":"+", speed);
    if (!true) { //free run here
        qWarning("could not free run along u axis");
    }
    return;
}

//------------------------------------------
void MotionHandler::endRunU()
{
    qInfo("stop free running along u axis");
    if (!true) { //stop free run here
        qWarning("could not stop free run along u axis");
    }
    return;
}

//******************************************
//gantry current position
std::vector<double> MotionHandler::whereAmI() {
    std::vector<double> position = {qQNaN(), qQNaN(), qQNaN(), qQNaN()};
    if (gantryConnected) {
        position = {qQNaN(), qQNaN(), qQNaN(), qQNaN()};
    }
    return position;
}
