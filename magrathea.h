#ifndef MAGRATHEA_H
#define MAGRATHEA_H

#include <QWidget>
#include <QTextEdit>
#include <opencv2/opencv.hpp>

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
#elif VALENCIA
class ACSCMotionHandler;
#endif

class Magrathea : public QWidget
{
    Q_OBJECT

public:
    explicit Magrathea(QWidget *parent = 0);
    ~Magrathea();

    static QTextEdit *outputLogTextEdit;

private slots:

    //timer
    void updatePosition();

    //camera
    void enableCameraBoxClicked(bool checked);
    void focusButtonClicked();
    void captureButtonClicked();
    void calibrationCaller(int input);
    void Calibration_ButtonClicked();
    void Calibration_2_ButtonClicked();

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

    cv::VideoCapture cap;
    cv::Mat sub_frame;

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
