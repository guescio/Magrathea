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

    static void motionInception();//TEST

public slots:

    virtual void motionTest();//TEST

    //******************************************
    // connect to the gantry
    virtual bool connectGantry(bool flag=true);
    virtual bool disconnectGantry();

    //******************************************
    // acknowledge gantry motion errors
    virtual void acknowledgeMotionFaultGantry();

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
    virtual void home();
    virtual void homeX();
    virtual void homeY();
    virtual void homeZ();
    virtual void homeU();

    //******************************************
    // absolute motion
    // NOTE units in mm, mm/s and deg/s
    virtual bool moveTo(double x, double y, double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void moveXTo(double x, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveYTo(double y, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveZTo(double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveUTo(double u, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // motion relative to current location
    // NOTE units in mm, mm/s and deg/s
    virtual bool moveBy(double x=0, double y=0, double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void moveXBy(double x=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveYBy(double y=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveZBy(double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual bool moveUBy(double u=0, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // free run
    virtual void runX(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void endRunX();
    virtual void runY(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void endRunY();
    virtual void runZ(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void endRunZ();
    virtual void runU(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void endRunU();

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
