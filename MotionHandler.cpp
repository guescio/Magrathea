#include "MotionHandler.h"
#include <QtMessageHandler>

//******************************************
MotionHandler::MotionHandler() :
    gantryConnected(false),
    axesEnabled(false)
    {}

//******************************************
MotionHandler::~MotionHandler() {}

//******************************************
// connect to the gantry

//------------------------------------------
bool MotionHandler::ConnectGantry()
{
    qInfo("connect gantry");
    gantryConnected=true;
    return true;
}

//------------------------------------------
bool MotionHandler::DisconnectGantry()
{
    qInfo("disconnect gantry");
    gantryConnected=false;
    return true;
}

//------------------------------------------
bool MotionHandler::EnableAxes()
{
    qInfo("enable axes");
    axesEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableAxes()
{
    qInfo("disable axes");
    axesEnabled=false;
    return true;
}

//******************************************
// home axes

//------------------------------------------
void MotionHandler::Home() {
    qInfo("home all axes");
	return; 
}

//------------------------------------------
void MotionHandler::HomeX() {
    qInfo("home x axis");
    return;
}

//------------------------------------------
void MotionHandler::HomeY() {
    qInfo("home y axis");
    return;
}

//------------------------------------------
void MotionHandler::HomeZ() {
    qInfo("home z axis");
    return;
}

//------------------------------------------
void MotionHandler::HomeU() {
    qInfo("home u axis");
    return;
}

//******************************************
// absolute motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::MoveTo(double x, double y, double z, double speed)
{
    qInfo("move to (%f mm, %f mm, %f mm) at %f mm/s speed", x, y, z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveXTo(double x, double speed) {
    qInfo("move x axis to %f mm at %f mm/s speed", x, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveYTo(double y, double speed) {
    qInfo("move y axis to %f mm at %f mm/s speed", y, speed);
    return;
}


//------------------------------------------
void MotionHandler::MoveZTo(double z, double speed) {
    qInfo("move z axis to %f mm at %f mm/s speed", z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveUTo(double u, double speed) {
    qInfo("move u axis to %f deg at %f deg/s speed", u, speed);
    return;
}

//******************************************
// relative  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::MoveBy(double x, double y, double z, double speed)
{
    qInfo("move by (%f mm, %f mm, %f mm) at %f mm/s speed", x, y, z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveXBy(double x, double speed) {
    qInfo("move x axis by %f mm at %f mm/s", x, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveYBy(double y, double speed) {
    qInfo("move y axis by %f mm at %f mm/s", y, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveZBy(double z, double speed) {
    qInfo("move z axis by %f mm at %f mm/s", z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveUBy(double u, double speed) {
    qInfo("move u axis by %f mm at %f mm/s", u, speed);
    return;
}


//******************************************
// free run

//------------------------------------------
void MotionHandler::RunX(double direction, double speed)
{
    qInfo("free running in %sx axis at %f mm/s", direction<0?"-":"+", speed);
    return;
}

//------------------------------------------
void MotionHandler::EndRunX()
{
    qInfo("stop free running in x axis");
    return;
}

//------------------------------------------
void MotionHandler::RunY(double direction, double speed)
{
    qInfo("free running in %sy...", direction<0?"-":"+");
    return;
}

//------------------------------------------
void MotionHandler::EndRunY()
{
    qInfo("stop free running in y");
    return;
}

//------------------------------------------
void MotionHandler::RunZ(double direction, double speed)
{
    qInfo("free running in %sz...", direction<0?"-":"+");
    return;
}

//------------------------------------------
void MotionHandler::EndRunZ()
{
    qInfo("stop free running in z");
    return;
}

//------------------------------------------
void MotionHandler::RunU(double direction, double speed)
{
    qInfo("free running in %su...", direction<0?"-":"+");
    return;
}

//------------------------------------------
void MotionHandler::EndRunU()
{
    qInfo("stop free running in u");
    return;
}

//******************************************
// default speeds
/*
//------------------------------------------
void MotionHandler::SetSpeedDefaults(double speedX, double speedY, double speedZ, double speedU)
{
    qInfo("setting speed defaults:");
    qInfo("\tx axis: %f mm/s", speedX);
    qInfo("\ty axis: %f mm/s", speedY);
    qInfo("\tz axis: %f mm/s", speedZ);
    qInfo("\tu axis: %f deg/s", speedU);
    return;
}

//------------------------------------------
void MotionHandler::SetSpeedX(double speed)
{
    qInfo("setting x axis speed to %f mm/s", speed);
    return;
}

//------------------------------------------
void MotionHandler::SetSpeedY(double speed)
{
    qInfo("setting y axis speed to %f mm/s", speed);
    return;
}

//------------------------------------------
void MotionHandler::SetSpeedZ(double speed)
{
    qInfo("setting z axis speed to %f mm/s", speed);
    return;
}

//------------------------------------------
void MotionHandler::SetSpeedU(double speed)
{
    qInfo("setting u axis speed to %f deg/s", speed);
    return;
}
*/

//******************************************
//gantry current position
std::vector<double> MotionHandler::WhereAmI() {
    std::vector<double> position = {0.0,0.0,0.0,0.0};
    return position;
}
