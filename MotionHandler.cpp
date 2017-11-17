#include "MotionHandler.h"
#include <QtMessageHandler>

//******************************************
MotionHandler::MotionHandler() :
    gantryConnected(false),
    axesEnabled(false),
    xAxisEnabled(false),
    yAxisEnabled(false),
    zAxisEnabled(false),
    uAxisEnabled(false)
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
    xAxisEnabled=true;
    yAxisEnabled=true;
    zAxisEnabled=true;
    uAxisEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::EnableXAxis()
{
    qInfo("enable x axis");
    xAxisEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::EnableYAxis()
{
    qInfo("enable y axis");
    yAxisEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::EnableZAxis()
{
    qInfo("enable z axis");
    zAxisEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::EnableUAxis()
{
    qInfo("enable u axis");
    uAxisEnabled=true;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableAxes()
{
    qInfo("disable axes");
    axesEnabled=false;
    xAxisEnabled=false;
    yAxisEnabled=false;
    zAxisEnabled=false;
    uAxisEnabled=false;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableXAxis()
{
    qInfo("disable x axis");
    xAxisEnabled=false;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableYAxis()
{
    qInfo("disable y axis");
    yAxisEnabled=false;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableZAxis()
{
    qInfo("disable z axis");
    zAxisEnabled=false;
    return true;
}

//------------------------------------------
bool MotionHandler::DisableUAxis()
{
    qInfo("disable u axis");
    uAxisEnabled=false;
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
    qInfo("move to (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed", x, y, z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveXTo(double x, double speed) {
    qInfo("move x axis to %.3f mm at %.3f mm/s speed", x, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveYTo(double y, double speed) {
    qInfo("move y axis to %.3f mm at %.3f mm/s speed", y, speed);
    return;
}


//------------------------------------------
void MotionHandler::MoveZTo(double z, double speed) {
    qInfo("move z axis to %.3f mm at %.3f mm/s speed", z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveUTo(double u, double speed) {
    qInfo("move u axis to %.3f deg at %.3f deg/s speed", u, speed);
    return;
}

//******************************************
// step  motion
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::MoveBy(double x, double y, double z, double speed)
{
    qInfo("move by (%.3f mm, %.3f mm, %.3f mm) at %.3f mm/s speed", x, y, z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveXBy(double x, double speed) {
    qInfo("move x axis by %.3f mm at %.3f mm/s", x, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveYBy(double y, double speed) {
    qInfo("move y axis by %.3f mm at %.3f mm/s", y, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveZBy(double z, double speed) {
    qInfo("move z axis by %.3f mm at %.3f mm/s", z, speed);
    return;
}

//------------------------------------------
void MotionHandler::MoveUBy(double u, double speed) {
    qInfo("move u axis by %.3f deg at %.3f deg/s", u, speed);
    return;
}


//******************************************
// free run
// NOTE units in mm, mm/s and deg/s

//------------------------------------------
void MotionHandler::RunX(double direction, double speed)
{
    qInfo("free running %sx axis at %.3f mm/s", direction<0?"-":"+", speed);
    return;
}

//------------------------------------------
void MotionHandler::EndRunX()
{
    qInfo("stop free running x axis");
    return;
}

//------------------------------------------
void MotionHandler::RunY(double direction, double speed)
{
    qInfo("free running %sy axis at %.3f mm/s", direction<0?"-":"+", speed);
    return;
}

//------------------------------------------
void MotionHandler::EndRunY()
{
    qInfo("stop free running y axis");
    return;
}

//------------------------------------------
void MotionHandler::RunZ(double direction, double speed)
{
    qInfo("free running %sz axis at %.3f mm/s", direction<0?"-":"+", speed);
    return;
}

//------------------------------------------
void MotionHandler::EndRunZ()
{
    qInfo("stop free running z axis");
    return;
}

//------------------------------------------
void MotionHandler::RunU(double direction, double speed)
{
    qInfo("free running %su axis at %.3f deg/s", direction<0?"-":"+", speed);
    return;
}

//------------------------------------------
void MotionHandler::EndRunU()
{
    qInfo("stop free running u axis");
    return;
}

//******************************************
//gantry current position
std::vector<double> MotionHandler::WhereAmI() {
    std::vector<double> position = {0.0,0.0,0.0,0.0};
    return position;
}
