#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>

namespace Ui {
    class CameraWidget;
}

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;
class QVBoxLayout;

class CameraWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit CameraWidget(QWidget *parent = 0);
        ~CameraWidget();

    private slots:
        void enableCameraBoxClicked(bool checked);
        void focusButtonClicked();
        void captureButtonClicked();

    private:
        Ui::CameraWidget *ui;
        QCamera *mCamera;
        QCameraViewfinder *mCameraViewfinder;
        QCameraImageCapture *mCameraImageCapture;
        QVBoxLayout *mCameraLayout;
};

#endif // CAMERAWIDGET_H
