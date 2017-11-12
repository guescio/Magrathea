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

    //gantry
    void connectGantryBoxClicked(bool clicked);
    void enableAxesBoxClicked(bool clicked);

    //joystick buttons
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

private:
    Ui::Magrathea *ui;

    QCamera *mCamera;
    QCameraViewfinder *mCameraViewfinder;
    QCameraImageCapture *mCameraImageCapture;
    QVBoxLayout *mCameraLayout;
    MotionHandler *mMotionHandler;

    int autoRepeatDelay;
};

#endif // MAGRATHEA_H
