#ifndef MAGRATHEA_H
#define MAGRATHEA_H

#include <QWidget>
#include <QTextEdit>

namespace Ui {
class Magrathea;
}

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;
class MotionHandler;
class Gantry_Vancouver;
class PetalAlgManager;
class GlueHandler;

class Magrathea : public QWidget
{
    Q_OBJECT

public:
    explicit Magrathea(QWidget *parent = 0);
    ~Magrathea();

    static QTextEdit *outputLogTextEdit;

private slots:

    //camera
    void enableCameraBoxToggled(bool toggled);
    void focusButtonClicked();
    void captureButtonClicked();

    //joystick buttons
    void connectJoystickBoxToggled(bool toggled);

    //x axis
    void positiveXButtonClicked();
    void negativeXButtonClicked();
    void xAxisStepContinousBoxToggled(bool flag);

    //y axis
    void positiveYButtonClicked();
    void negativeYButtonClicked();
    void yAxisStepContinousBoxToggled(bool flag);

    //z axis
    void positiveZButtonClicked();
    void negativeZButtonClicked();
    void zAxisStepContinousBoxToggled(bool flag);

    //u axis
    void positiveUButtonClicked();
    void negativeUButtonClicked();
    void uAxisStepContinousBoxToggled(bool flag);

    // for algorithms
    void InnerFiducialButtonPressed();
    void InnerFoundButtonToggled(bool flag);
    void OuterFiducialButtonPressed();
    void OuterFoundButtonToggled(bool flag);

    void SensorPullDownActivated(int index);
    void SensorFirstFoundButtonToggled(bool flag);
    void SensorSecondFoundButtonToggled(bool flag);
    void PickUpButtonPressed();

    void PetalPullDownActivated(int index);
    void FiducialPullDownActivated(int index);
    void ReviewButtonPressed();
    void PreviousFidButtonPressed();
    void NextFidButtonPressed();

    void glueButtonClicked();


private:
    Ui::Magrathea *ui;

    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mCameraLayout;
    MotionHandler *mMotionHandler;
    Gantry_Vancouver *mGantryVancouver;

    PetalAlgManager * mPetalManager;
    //GlueHandler * mGlueHandler;

    int autoRepeatDelay;

    // Step sizes in mm for motion
    double mStepDefault;
    double mStepSmall;
    double mStepTiny;
    double mStep;

};


#endif // MAGRATHEA_H

