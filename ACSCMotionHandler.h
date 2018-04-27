#ifndef ACSCMOTIONHANDLER_H
#define ACSCMOTIONHANDLER_H

#include "MotionHandler.h"
#include "ACSC/C_CPP/ACSC.h"
#include <QWidget>

class ACSCMotionHandler : public MotionHandler
{
    Q_OBJECT

public:

    ACSCMotionHandler();
    ~ACSCMotionHandler();

    bool gantryConnected;
    bool xAxisEnabled;
    bool yAxisEnabled;
    bool zAxisEnabled;
    bool uAxisEnabled;

public slots:

    //******************************************
    // connect to the gantry
    virtual bool connectGantry(bool flag=true);
    virtual bool disconnectGantry();

    // enable axes before any movement
    virtual bool enableAxes(bool flag=true);
    virtual bool enableXAxis(bool flag=true);
    virtual bool enableYAxis(bool flag=true);
    virtual bool enableZAxis(bool flag=true);
    virtual bool enableUAxis(bool flag=true);
    virtual bool disableAxes();
    virtual bool disableXAxis();
    virtual bool disableYAxis();
    virtual bool disableZAxis();
    virtual bool disableUAxis();

    //******************************************
    // gantry current position
    virtual std::vector<double> whereAmI();

    //******************************************
    // home axes
    virtual bool home();
    virtual bool homeX();
    virtual bool homeY();
    virtual bool homeZ();
    virtual bool homeU();

    //******************************************
    // absolute motion
    // NOTE units in mm, mm/s and deg/s
    virtual bool moveTo(double x, double y, double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveXTo(double x, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveYTo(double y, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveZTo(double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveUTo(double u, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // motion relative to current location
    // NOTE units in mm, mm/s and deg/s
    virtual bool moveBy(double x=0, double y=0, double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveXBy(double x=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveYBy(double y=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveZBy(double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveUBy(double u=0, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // free run
    virtual bool runX(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool endRunX();
    virtual bool runY(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool endRunY();
    virtual bool runZ(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool endRunZ();
    virtual bool runU(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool endRunU();

signals:

    void updatePositions_s();

private:

    HANDLE gantry;
    const int X_axis = ACSC_AXIS_0;
    const int Y_axis = ACSC_AXIS_1;
    const int Z_axis = ACSC_AXIS_2;
    const int U_axis = ACSC_AXIS_3;

    const int TimeOut = 10000;//timeout in ms

    double Home_coord[4]          = {0.,0.,0.,0.};
    double default_speed         = 1000;
    double default_angular_speed = 10;

//    A3200Handle gantry;

//    AXISMASK xaxis;
//    AXISINDEX xindex;

//    AXISMASK yaxis;
//    AXISINDEX yindex;

//    AXISMASK zaxis;
//    AXISINDEX zindex;

//    AXISMASK uaxis;
//    AXISINDEX uindex;

//    AXISMASK allAxes;
//    AXISMASK xyAxes;
};

#endif //ACSCMOTIONHANDLER_H
