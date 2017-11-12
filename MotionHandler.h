#pragma once
#include <vector>
#include <QWidget>

class MotionHandler : public QWidget
{
    Q_GADGET

public:

	MotionHandler();
	~MotionHandler();

public slots:

    // Turn the gantry on
    virtual void ActivateGantry(bool toggled);

    // Need to activate axes before machine can be used
    virtual void ActivateAllAxes(bool toggled);

    // Home all axes
    virtual void Home();
};
