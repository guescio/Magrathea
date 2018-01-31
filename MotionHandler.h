#pragma once
#include <QWidget>

class MotionHandler : public QWidget
{
    Q_GADGET

public:

	MotionHandler();
	~MotionHandler();

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

    //******************************************
    // acknowledge gantry motion errors
    virtual bool acknowledgeMotionFaultGantry();

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

    //******************************************
    // default speeds
    // NOTE default unit is mm/s
    /*
    virtual void SetSpeedDefaults(double speedX=std::numeric_limits<double>::quiet_NaN(),
                                  double speedY=std::numeric_limits<double>::quiet_NaN(),
                                  double speedZ=std::numeric_limits<double>::quiet_NaN(),
                                  double speedU=std::numeric_limits<double>::quiet_NaN());
    virtual void SetSpeedX(double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void SetSpeedY(double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void SetSpeedZ(double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void SetSpeedU(double speed=std::numeric_limits<double>::quiet_NaN());
    */
};
