#pragma once
#include "MotionHandler.h"
#include "A3200.h"
#include <iostream>
#include <tchar.h>
#include <math.h>

class Gantry_Vancouver :
	public MotionHandler
{
public:

	Gantry_Vancouver();
	~Gantry_Vancouver();

    ///////////////////////////////
    // Starting and finishing
    // Turn the gantry on

	void ActivateGantry();

	// Need to activate axes before machine can be used
	void ActivateAllAxes();

    ///////////////////////////////
    // Motion commands

	// Home all axes
    void Home();

    // Connect joystick
    void ConnectJoystick();

    // Disconnect joystick
    void DisconnectJoystick();

    // Home individual axis (X)
    void HomeX();

    // Home individual axis (Y)
    void HomeY();

    // Home individual axis (Z)
    void HomeZ();

    // Home individual axis (U)
    void HomeU();

    // Move to absolute location specified by x, y, z
    // Note default unit is mm
    void MoveTo(double x, double y, double z, double u, double speed = NAN);

    // Move only in z (useful for autofocus)
    void MoveZTo(double z, double speed = NAN);

    // Move relative to current location by an amount x, y, z
    // Note default unit is mm
    void MoveBy(double x, double y, double z, double u, double speed = NAN);

    // Move only in z (useful for autofocus)
    void MoveZBy(double z, double speed = NAN);

    // Begin free running (x direction)
    void RunX();

    // Begin free running (negative x direction)
    void RunXNeg();

    // Stop free running (x direction)
    void EndRunX();

    // Begin free running (y direction)
    void RunY();

    // Begin free running (negative y direction)
    void RunYNeg();

    // Stop free running (y direction)
    void EndRunY();

    // Begin free running (z direction)
    void RunZ();

    // Begin free running (negative z direction)
    void RunZNeg();

    // Stop free running (z direction)
    void EndRunZ();

    // Begin free running (rotational direction)
    void RunU();

    // Begin free running (rotate the other way)
    void RunUNeg();

    // Stop free running (rotational direction)
    void EndRunU();

    ///////////////////////////////
    // Configuration commands

    // Set default speed for each axis
    void SetSpeedDefaults(double speedX, double speedY, double speedZ, double speedU);

    // Set speed for x axis
    void SetSpeedX(double speed);

    // Set speed for y axis
    void SetSpeedY(double speed);

    // Set speed for z axis
    void SetSpeedZ(double speed);

    // Set speed for u axis
    void SetSpeedU(double speed);

    ///////////////////////////////
    // Status commands

    // Returns current location as vector in x, y, z, u
    std::vector<double> WhereAmI();

private :

	// Handle to give access to A3200
	A3200Handle gantry;

	AXISMASK xaxis;
	AXISINDEX xindex;

	AXISMASK yaxis;
	AXISINDEX yindex;

	AXISMASK zaxis;
	AXISINDEX zindex;

	AXISMASK uaxis;
	AXISINDEX uindex;

	AXISMASK allAxes;
	AXISMASK xyAxes;

	double default_xspeed;
	double default_yspeed;
	double default_zspeed;
	double default_uspeed;
};
