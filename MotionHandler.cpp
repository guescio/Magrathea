#include "MotionHandler.h"
#include <QtMessageHandler>
#include <QDebug>

MotionHandler::MotionHandler() {}

MotionHandler::~MotionHandler() {}

void MotionHandler::ActivateGantry()
{
    qInfo("activate gantry");
	return;
}

void MotionHandler::ActivateAllAxes()
{
    qInfo("activate all axes");
	return;
}

void MotionHandler::Home() { 
    qInfo("homing all axes");
	return; 
}

void MotionHandler::ConnectJoystick() {

    qInfo("connecting joystick");
    return;
}

void MotionHandler::DisconnectJoystick() {

    qInfo("returning motion control to GUI");
    return;

}
void MotionHandler::HomeX() {

    qInfo("Homed X axis.");
    return;
}
void MotionHandler::HomeY() {

    qInfo("Homed Y axis.");
    return;
}
void MotionHandler::HomeZ() {

    qInfo("Homed Z axis.");
    return;
}
void MotionHandler::HomeU() {

    qInfo("Homed U axis.");
    return;
}
void MotionHandler::MoveTo(double x, double y, double z, double u, double speed)
{
    qInfo() << "Moved axes to" << x << y << z << "at speed" << speed;
    return;
}

void MotionHandler::MoveZTo(double z, double speed) {
    qInfo() << "Moved z axis to" << z << "at speed" << speed;
    return;
}


// Move relative to current location by an amount x, y, z
// Note default unit is mm
void MotionHandler::MoveBy(double x, double y, double z, double u, double speed)
{
    qInfo() << "Moved axes by" << x << y << z << "at speed" << speed;
    return;
}

void MotionHandler::MoveZBy(double z, double speed) {
    qInfo() << "Moved z axis by" << z << "at speed" << speed;
    return;
}

// Begin free running (x direction)
void MotionHandler::RunX()
{
    qInfo("Free running in X...");
    return;
}

// Begin free running (negative x direction)
void MotionHandler::RunXNeg()
{
    qInfo("Free running in negative X...");
    return;
}

// Stop free running (x direction)
void MotionHandler::EndRunX()
{
    qInfo("Stopped free running in X");
    return;
}

// Begin free running (y direction)
void MotionHandler::RunY()
{
    qInfo("Free running in Y...");
    return;
}

// Begin free running (negative y direction)
void MotionHandler::RunYNeg()
{
    qInfo("Free running in negative Y...");
    return;
}

// Stop free running (y direction)
void MotionHandler::EndRunY()
{
    qInfo("Stopped free running in Y");
    return;
}

// Begin free running (z direction)
void MotionHandler::RunZ()
{
    qInfo("Free running in Z...");
    return;
}

// Begin free running (negative z direction)
void MotionHandler::RunZNeg()
{
    qInfo("Free running in negative Z...");
    return;
}

// Stop free running (z direction)
void MotionHandler::EndRunZ()
{
    qInfo("Stopped free running in Z");
    return;
}

// Begin free running (rotational direction)
void MotionHandler::RunU()
{
    qInfo("Free running rotationally...");
    return;
}

// Begin free running (rotate the other way)
void MotionHandler::RunUNeg()
{
    qInfo("Free running rotationally (backwards) ...");
    return;
}

// Stop free running (rotational direction)
void MotionHandler::EndRunU()
{
    qInfo("Stopped free running rotationally");
    return;
}

void MotionHandler::SetSpeedDefaults(double speedX, double speedY, double speedZ, double speedU)
{
    qInfo("Setting speed defaults to:");
    qInfo() <<"\nx axis:" << speedX;
    qInfo() << "\ny axis:" << speedY;
    qInfo() << "\nz axis:" << speedZ;
    qInfo() << "\nrotational axis:" << speedU;
    return;
}

void MotionHandler::SetSpeedX(double speed)
{
    qInfo() << "Setting x axis speed to" << speed;
    return;
}

void MotionHandler::SetSpeedY(double speed)
{
    qInfo() << "Setting y axis speed to" << speed;
    return;
}

void MotionHandler::SetSpeedZ(double speed)
{
    qInfo() << "Setting z axis speed to" << speed;
    return;
}

void MotionHandler::SetSpeedU(double speed)
{
    qInfo() << "Setting rotational speed to" << speed;
    return;
}

std::vector<double> MotionHandler::WhereAmI() {

    qInfo() << "Meaningless request in base class.";
    std::vector<double> empty = {0.0,0.0,0.0,0.0};
    return empty;
}
