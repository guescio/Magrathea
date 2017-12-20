#ifndef PETALALGMANAGER_H
#define PETALALGMANAGER_H

#include <iostream>

#include "Gantry_Vancouver.h"
#include<cmath>
#include<QtMath>
#include<windows.h>
#include "MotionHandler.h"
//#include "GlueHandler.h"

class PetalAlgManager
{
public:
    PetalAlgManager(MotionHandler *gantryToUse);
    ~PetalAlgManager() {};

    void InnerFiducial();
    void StoreInner();
    void OuterFiducial();
    void StoreOuter();
    void whichSensor(std::string sensor, int joystick);
    void StoreFirst();
    void StoreSecond();
    void PickUp();
    void review();

protected:
    /*** Constants ***/
    const double PI = M_PI;
    const double glueHeight = 0.120;	// Height of glue layer (mm)
    const double camDist = 40.000;		// Distance from focal point to edge of camera (mm)

    // holder->camera offset
    const double camX = 131.9050;
    const double camY = 0.0000;
    const double camZ = 20.0000;

    // Coordinates of petal origin/fiducial
    double petalPos1X = 19.8536;
    double petalPos1Y = -723.0145;
    double petalPos1Z = -109.2500;

    // Coordinates of 2nd petal fiducial
    double petalPos2X = 613.0820;
    double petalPos2Y = -591.442;
    double petalPos2Z = -109.209;

    // Angle of petal (degrees
    const double petalAngle = 12.4828;

    /*** Variables ***/
    double fidAngle, holdAngle, endLength, endAngle, offAngle, offLength, Angle;
    double dummy;
    double fidXYZ1[3], fidXYZ2[3], current[3], final[3], fid[4][2];

    //MotionHandler call;
//    Gantry_Vancouver call;
    MotionHandler * m_Gantry;
    std::vector<double> position;
};


#endif // PETALALGMANAGER_H
