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
class QTimer;

#ifdef AEROTECH
class AerotechMotionHandler;
#endif

class Magrathea : public QWidget
{
    Q_OBJECT

public:
    explicit Magrathea(QWidget *parent = 0);
    ~Magrathea();

    static QTextEdit *outputLogTextEdit;

private slots:

    //TEST
    void test();//TEST

    //timer
    void updatePosition();

    //camera
    void enableCameraBoxClicked(bool checked);
    void focusButtonClicked();
    void captureButtonClicked();

    //gantry
    void connectGantryBoxClicked(bool checked);
    void enableAxesClicked(bool checked);

    //motion
    void enableJoystickFreeRun(bool checked);
    void freeRun();
    void enableJoystickStepMotion(bool checked);
    void stepMotion();
    void positionMove();
    void axisStepRepeatBoxClicked(bool checked);

private:
    Ui::Magrathea *ui;

    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mCameraLayout;
    MotionHandler *mMotionHandler;
    QTimer *mPositionTimer;

    int autoRepeatDelay;
    int autoRepeatInterval;
};

#endif // MAGRATHEA_H
