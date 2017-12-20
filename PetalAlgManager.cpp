#define _USE_MATH_DEFINES

#include "PetalAlgManager.h"

PetalAlgManager::PetalAlgManager(MotionHandler *gantryToUse)
{
     m_Gantry = gantryToUse;

  /*   // Petal core locations
     m_xPOrigin = NULL;
     m_yPOrigin = NULL;
     m_xPDistant = NULL;
     m_yPDistant = NULL;
     m_rPDistant = NULL; // cross check: should be very close to nominal length
     m_thetaPDistant = NULL; // cross check: angle from origin to distant fiducial
     m_petalAngle = NULL; // angle off of when petal perfectly along x

     // Locations of F fiducial:
     // module 0
     m_mod0_f1x = NULL;
     m_mod0_f1y = NULL;
     m_mod0_f2x = NULL;
     m_mod0_f2y = NULL;
     m_mod0_f3x = NULL;
     m_mod0_f3y = NULL;
     m_mod0_f4x = NULL;
     m_mod0_f4y = NULL;

     // NOMINAL LOCATIONS
     // THESE SHOULD BE UNIVERSAL
     // AND TAKEN FROM CONFIG FILE
     m_nominal_petalLength = NULL;
     m_nominal_petalHeight = NULL;
     m_nominal_petalAngle = 12.4616;
     m_nominal_petalR = 607.5704;

     // NOMINAL LOCATIONS: FIDUCIALS
     // OF MODULE 0
     m_nominal_mod0_f1R = 36.9660;
     m_nominal_mod0_f1ang = 78.1448;
     m_nominal_mod0_f2R = 120.7649;
     m_nominal_mod0_f2ang = 23.5009;
     m_nominal_mod0_f3R = 121.1768;
     m_nominal_mod0_f3ang = 335.8958;
     m_nominal_mod0_f4R = 41.5251;
     m_nominal_mod0_f4ang = 279.8646;*/
}

void PetalAlgManager::InnerFiducial()
{
    // Move to first petal fiducial
    m_Gantry->MoveTo(petalPos1X, petalPos1Y, 0, 0);
    m_Gantry->MoveTo(petalPos1X, petalPos1Y, petalPos1Z, 0);
    std::cout << "moved to position" << std::endl;

    // Initialize Joystick
    //m_Gantry->ConnectJoystick();
    std::cout << "joystick connected" << std::endl;

    return;
}

void PetalAlgManager::StoreInner()
{
    // Store absolute fiducial position
    position = m_Gantry->WhereAmI();
    petalPos1X = position[0];
    petalPos1Y = position[1];
    petalPos1Z = position[2];

    return;
}

void PetalAlgManager::OuterFiducial()
{
    // Move to fiducial coordinates (top-right)
    m_Gantry->MoveTo(petalPos2X, petalPos2Y, petalPos2Z, 0);

    //use joystick again...
    m_Gantry->ConnectJoystick();

    return;
}

void PetalAlgManager::StoreOuter()
{
    // Store absolute fiducial position (2nd)
    position = m_Gantry->WhereAmI();
    petalPos2X = position[0];
    petalPos2Y = position[1];
    petalPos2Z = position[2];

    return;
}

void PetalAlgManager::whichSensor(std::string sensor, int joystick)
{
    const char * c = sensor.c_str();

    if(strcmp(c,"R0"))
    {
        // Move to rough sensor location
        //m_Gantry->MoveTo(senX, senY, senZ);

        // Establish sensor values
        endAngle = 1.4518;		// Angle between sensor placement and petal angle
        endLength = 60.1748;	// Length from origin to location of sensor

        offLength = 68.8670;	// Total distance from bottom-right fiducial to centre of holder (mm)
        offAngle = 4.3982;		// Angle between above line and line between measured fiducials (degrees)
        holdAngle = 39.7460;	// Angle between measured fiducals and position of holder (degrees

        // Sensor fiducial measurements.
        // Measured from the petal origin:
        fid[0][0] = 36.7661;	// TL Length
        fid[0][1] = 79.7308;	// TL Angle

        fid[1][0] = 119.8148;	// TR Length
        fid[1][1] = 23.6986;	// TR Angle

        fid[2][0] = 120.2308;	// BR Length
        fid[2][1] = 335.6939;	// BR Angle

        fid[3][0] = 41.3600;	// BL Length
        fid[3][1] = 278.4449;	// BL Angle
    }

    else if(strcmp(c,"R1"))
    {

    }

    else if(strcmp(c,"R2"))
    {

    }

    // etc....

    // Use joystick to improve location
    if (joystick == 1)
        m_Gantry->ConnectJoystick();

    return;
}

void PetalAlgManager::StoreFirst()
{
    // Obtain fiducial coordinates (bottom-left)
    position = m_Gantry->WhereAmI();
    fidXYZ1[0] = position[0];
    fidXYZ1[1] = position[1];
    fidXYZ1[2] = position[2];
}

void PetalAlgManager::StoreSecond()
{
    // Obtain fiducial coordinates (top-right)
    position = m_Gantry->WhereAmI();
    fidXYZ2[0] = position[0];
    fidXYZ2[1] = position[1];
    fidXYZ2[2] = position[2];

    return;
}

void PetalAlgManager::PickUp()
{
    // Calculate angle of petal fiducial markers
    fidAngle = (atan2((petalPos2Y - petalPos1Y), (petalPos2X - petalPos1X))) * 180/PI;

    // Calculate rotation and shift of holder, and move gantry to calculated location
    Angle = (atan2((fidXYZ2[1] - fidXYZ1[1]), (fidXYZ2[0] - fidXYZ1[0])) * 180 / PI);
    m_Gantry->MoveBy(offLength*cos((180 + Angle - offAngle)*PI / 180) + camX, offLength*sin((180 + Angle - offAngle)*PI / 180) + camY, 0, Angle + holdAngle);

    // Obtain current coordiinates
    position = m_Gantry->WhereAmI();
    current[0] = position[0];
    current[1] = position[1];
    current[2] = position[2];

    // Lower holder to just above highest fiducial height
    if (fidXYZ2[2] > fidXYZ1[2])
        m_Gantry->MoveZTo(fidXYZ2[2] - camDist + camZ + 5, 25);
    else
        m_Gantry->MoveZTo(fidXYZ1[2] - camDist + camZ + 5, 25);

    m_Gantry->MoveZBy(-5, 1);

    /*************/
    /* VACUUM ON */
    /*************/

    // Lift sensor up
    m_Gantry->MoveZBy(5, 1);
    m_Gantry->HomeZ(); // Or change to lower height with MoveZTo(...)

    // Move to sensor placement on petal
    m_Gantry->MoveTo(petalPos1X + endLength*cos((fidAngle - petalAngle - endAngle)*PI / 180), petalPos1Y + endLength*sin((fidAngle - petalAngle - endAngle)*PI / 180), 0, fidAngle - petalAngle);
    m_Gantry->MoveZTo(petalPos2Z + 3.5 - camDist + camZ + 5, 25);
    m_Gantry->MoveZTo(petalPos2Z + 3.5 - camDist + camZ + glueHeight, 1);

    // Wait (for glue to set - in ms)
    //Sleep(5 * 60 * 1000);	// minutes * 60 * 1000

    /**************/
    /* VACUUM OFF */
    /**************/

    m_Gantry->MoveZBy(40, 25);

    return;
}

void PetalAlgManager::review()
{
    return;
}
