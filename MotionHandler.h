#pragma once

#include <limits>
#include <QWidget>

class MotionHandler : public QWidget
{
    Q_GADGET

public:

	MotionHandler();
	~MotionHandler();

    bool gantryConnected;
    bool axesEnabled;

public slots:

    //******************************************
    // connect to the gantry
    virtual bool ConnectGantry();
    virtual bool DisconnectGantry();

    // enable axes before any movement
    virtual bool EnableAxes();
    virtual bool DisableAxes();

    //******************************************
    // gantry current position
    virtual std::vector<double> WhereAmI();

    //******************************************
    // home axes
    virtual void Home();
    virtual void HomeX();
    virtual void HomeY();
    virtual void HomeZ();
    virtual void HomeU();

    //******************************************
    // absolute motion
    // NOTE units in mm, mm/s and deg/s
    virtual void MoveTo(double x, double y, double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveXTo(double x, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveYTo(double y, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveZTo(double z, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveUTo(double u, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // motion relative to current location
    // NOTE units in mm, mm/s and deg/s
    virtual void MoveBy(double x=0, double y=0, double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveXBy(double x=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveYBy(double y=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveZBy(double z=0, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void MoveUBy(double u=0, double speed=std::numeric_limits<double>::quiet_NaN());

    //******************************************
    // free run
    virtual void RunX(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void EndRunX();
    virtual void RunY(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void EndRunY();
    virtual void RunZ(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void EndRunZ();
    virtual void RunU(double direction, double speed=std::numeric_limits<double>::quiet_NaN());
    virtual void EndRunU();

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
