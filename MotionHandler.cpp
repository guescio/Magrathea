#include "MotionHandler.h"
#include <QtMessageHandler>

MotionHandler::MotionHandler() {}

MotionHandler::~MotionHandler() {}

void MotionHandler::ActivateGantry(bool toggled)
{
    if (toggled)
        qInfo("activate gantry");
    else
        qInfo("disactivate gantry");
	return;
}

void MotionHandler::ActivateAllAxes(bool toggled)
{
    if (toggled)
        qInfo("activate all axes");
    else
        qInfo("disactivate all axes");
	return;
}

void MotionHandler::Home() { 
    qInfo("homing all axes");
	return; 
}
