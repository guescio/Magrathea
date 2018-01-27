#include "magrathea.h"
#include "ui_magrathea.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QtMessageHandler>
#include <MotionHandler.h>
#include <cmath>
#ifdef VANCOUVER
#include <AerotechMotionhandler.h>
#endif

//******************************************
Magrathea::Magrathea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Magrathea)
{
    ui->setupUi(this);

    //------------------------------------------
    //output log
    outputLogTextEdit = new QTextEdit;
    outputLogTextEdit->setReadOnly(true);
    ui->leftTabWidget->addTab(outputLogTextEdit, "log");
    ui->leftTabWidget->setCurrentWidget(outputLogTextEdit);

    //------------------------------------------
    #ifdef VANCOUVER
        qInfo("Vancouver, Aerotech A3200 gantry");
        mMotionHandler = new AerotechMotionhandler();
    #else
        qInfo("where is your gantry?");
        mMotionHandler = new MotionHandler();
    #endif

    //------------------------------------------
    //camera

    //create camera objects
    mCamera = new QCamera(this);
    mCameraViewfinder = new QCameraViewfinder(this);
    mCameraImageCapture = new QCameraImageCapture(mCamera, this);
    mCameraLayout = new QVBoxLayout;

    //add the camera to the layout
    mCamera->setViewfinder(mCameraViewfinder);
    mCameraLayout->addWidget(mCameraViewfinder);
    mCameraLayout->setContentsMargins(0,0,0,0);

    //add the layout to the frame area in the GUI
    ui->frame->setLayout(mCameraLayout);

    //------------------------------------------
    //gantry
    autoRepeatDelay=1000;//ms
    autoRepeatInterval=1000;//ms
    ui->enableAxesButton->setEnabled(false);
    ui->disableAxesButton->setEnabled(false);
    ui->xAxisEnableBox->setEnabled(false);
    ui->yAxisEnableBox->setEnabled(false);
    ui->zAxisEnableBox->setEnabled(false);
    ui->uAxisEnableBox->setEnabled(false);
    ui->resetErrorButton->setEnabled(false);
    enableAxesClicked(false);
    //ui->freeRunRadioButton->setChecked(true);

    //------------------------------------------
    //position tab
    ui->xAxisPositionLine2->setReadOnly(true);
    ui->yAxisPositionLine2->setReadOnly(true);
    ui->zAxisPositionLine2->setReadOnly(true);
    ui->uAxisPositionLine2->setReadOnly(true);

    //------------------------------------------
    //navigation tab
    ui->xAxisPositionLine->setReadOnly(true);
    ui->yAxisPositionLine->setReadOnly(true);
    ui->zAxisPositionLine->setReadOnly(true);
    ui->uAxisPositionLine->setReadOnly(true);

    //------------------------------------------
    //connect signals and slots

    //camera
    connect(ui->enableCameraBox, SIGNAL(toggled(bool)), this, SLOT(enableCameraBoxClicked(bool)));
    connect(ui->focusButton,     SIGNAL(clicked(bool)), this, SLOT(focusButtonClicked()));
    connect(ui->captureButton,   SIGNAL(clicked(bool)), this, SLOT(captureButtonClicked()));

    //gantry
    connect(ui->connectGantryBox, SIGNAL(toggled(bool)), this, SLOT(connectGantryBoxClicked(bool)));
    connect(ui->enableAxesButton, SIGNAL(clicked(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->disableAxesButton,SIGNAL(clicked(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->xAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->yAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->zAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->uAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->resetErrorButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::acknowledgeMotionFaultGantry);

    //joystick
    connect(ui->freeRunRadioButton, SIGNAL(clicked(bool)), this, SLOT(enableJoystickFreeRun(bool)));
    connect(ui->stepRadioButton,    SIGNAL(clicked(bool)), this, SLOT(enableJoystickStepMotion(bool)));

    //home axes
    connect(ui->axesHomeButton,  &QPushButton::clicked, mMotionHandler, &MotionHandler::home);
    connect(ui->xAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::homeX);
    connect(ui->yAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::homeY);
    connect(ui->zAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::homeZ);
    connect(ui->uAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::homeU);

    //position move
    connect(ui->xAxisPositionMoveButton, SIGNAL(clicked(bool)), this, SLOT(positionMove()));
    connect(ui->yAxisPositionMoveButton, SIGNAL(clicked(bool)), this, SLOT(positionMove()));
    connect(ui->zAxisPositionMoveButton, SIGNAL(clicked(bool)), this, SLOT(positionMove()));
    connect(ui->uAxisPositionMoveButton, SIGNAL(clicked(bool)), this, SLOT(positionMove()));

    //step motion
    connect(ui->xAxisStepMoveButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
    connect(ui->yAxisStepMoveButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
    connect(ui->zAxisStepMoveButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
    connect(ui->uAxisStepMoveButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));

    //step motion autorepeat box
    connect(ui->xAxisStepRepeatBox, SIGNAL(clicked(bool)), this, SLOT(axisStepRepeatBoxClicked(bool)));
    connect(ui->yAxisStepRepeatBox, SIGNAL(clicked(bool)), this, SLOT(axisStepRepeatBoxClicked(bool)));
    connect(ui->zAxisStepRepeatBox, SIGNAL(clicked(bool)), this, SLOT(axisStepRepeatBoxClicked(bool)));
    connect(ui->uAxisStepRepeatBox, SIGNAL(clicked(bool)), this, SLOT(axisStepRepeatBoxClicked(bool)));
}

//******************************************
Magrathea::~Magrathea()
{
    delete ui;
    delete mMotionHandler;
}

//******************************************
//camera

//------------------------------------------
//enable
void Magrathea::enableCameraBoxClicked(bool clicked)
{
    if (clicked) mCamera->start();
    else mCamera->stop();
    return;
}

//------------------------------------------
//focus
void Magrathea::focusButtonClicked()
{
    qInfo("camera focus");
    return;
}

//------------------------------------------
//capture picture
void Magrathea::captureButtonClicked()
{
    auto filename = QFileDialog::getSaveFileName(this, "Capture", "/ ", "Image (*.jpg;*.jpeg)");
    if (filename.isEmpty()) {
        return;
    }
    mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings imageEncoderSettings;
    imageEncoderSettings.setCodec("image/jpeg");
    imageEncoderSettings.setResolution(1600, 1200);
    mCameraImageCapture->setEncodingSettings(imageEncoderSettings);
    mCamera->setCaptureMode(QCamera::CaptureStillImage);
    mCamera->start();
    mCamera->searchAndLock();
    mCameraImageCapture->capture(filename);
    mCamera->unlock();
    return;
}

//******************************************
//gantry

//------------------------------------------
//connect gantry
//proxy function to handle the ConnectGantry and DisconnectGanrty functions from MotionHandler
void Magrathea::connectGantryBoxClicked(bool checked)
{
    if (checked)
    {
        if (mMotionHandler->connectGantry()) {
            ui->enableAxesButton->setEnabled(true);
            ui->disableAxesButton->setEnabled(true);
            ui->xAxisEnableBox->setEnabled(true);
            ui->yAxisEnableBox->setEnabled(true);
            ui->zAxisEnableBox->setEnabled(true);
            ui->uAxisEnableBox->setEnabled(true);
            ui->resetErrorButton->setEnabled(true);
        } else {
            ui->connectGantryBox->setChecked(false);
            qWarning("could not connect to gantry");
        }
    } else {
        if (mMotionHandler->xAxisEnabled || mMotionHandler->yAxisEnabled || mMotionHandler->zAxisEnabled || mMotionHandler->uAxisEnabled) {
            ui->connectGantryBox->setChecked(true);
            qWarning("disable axes before disconnecting from gantry");
        } else {
            if(mMotionHandler->disconnectGantry()) {
                ui->enableAxesButton->setEnabled(false);
                ui->disableAxesButton->setEnabled(false);
                ui->xAxisEnableBox->setEnabled(false);
                ui->yAxisEnableBox->setEnabled(false);
                ui->zAxisEnableBox->setEnabled(false);
                ui->uAxisEnableBox->setEnabled(false);
                ui->resetErrorButton->setEnabled(false);
            } else {
                ui->connectGantryBox->setChecked(true);
                qWarning("could not disconnect from gantry");
            }
        }
    }
}

//------------------------------------------
//enable axes
//proxy function to handle the EnableAxes and DisableAxes functions from MotionHandler
void Magrathea::enableAxesClicked(bool checked)
{
    if (sender() == ui->enableAxesButton) {
        ui->xAxisEnableBox->setChecked(true);
        ui->yAxisEnableBox->setChecked(true);
        ui->zAxisEnableBox->setChecked(true);
        ui->uAxisEnableBox->setChecked(true);
    } else if (sender() == ui->disableAxesButton) {
        ui->xAxisEnableBox->setChecked(false);
        ui->yAxisEnableBox->setChecked(false);
        ui->zAxisEnableBox->setChecked(false);
        ui->uAxisEnableBox->setChecked(false);
    } else if (sender() == ui->xAxisEnableBox) mMotionHandler->enableXAxis(checked);
      else if (sender() == ui->yAxisEnableBox) mMotionHandler->enableYAxis(checked);
      else if (sender() == ui->zAxisEnableBox) mMotionHandler->enableZAxis(checked);
      else if (sender() == ui->uAxisEnableBox) mMotionHandler->enableUAxis(checked);

    //gantry connection
    ui->connectGantryBox->setEnabled(!(mMotionHandler->xAxisEnabled ||
                                       mMotionHandler->yAxisEnabled ||
                                       mMotionHandler->zAxisEnabled ||
                                       mMotionHandler->uAxisEnabled));

    //joystick
    ui->leftTabWidget->widget(0)->setEnabled(mMotionHandler->xAxisEnabled ||
                                             mMotionHandler->yAxisEnabled ||
                                             mMotionHandler->zAxisEnabled ||
                                             mMotionHandler->uAxisEnabled);
    ui->positiveXButton->setEnabled(mMotionHandler->xAxisEnabled);
    ui->negativeXButton->setEnabled(mMotionHandler->xAxisEnabled);
    ui->positiveYButton->setEnabled(mMotionHandler->yAxisEnabled);
    ui->negativeYButton->setEnabled(mMotionHandler->yAxisEnabled);
    ui->positiveZButton->setEnabled(mMotionHandler->zAxisEnabled);
    ui->negativeZButton->setEnabled(mMotionHandler->zAxisEnabled);
    ui->positiveUButton->setEnabled(mMotionHandler->uAxisEnabled);
    ui->negativeUButton->setEnabled(mMotionHandler->uAxisEnabled);

    //home
    ui->axesHomeButton->setEnabled(mMotionHandler->xAxisEnabled ||
                                   mMotionHandler->yAxisEnabled ||
                                   mMotionHandler->zAxisEnabled ||
                                   mMotionHandler->uAxisEnabled);
    ui->xAxisHomeButton->setEnabled(mMotionHandler->xAxisEnabled);
    ui->yAxisHomeButton->setEnabled(mMotionHandler->yAxisEnabled);
    ui->zAxisHomeButton->setEnabled(mMotionHandler->zAxisEnabled);
    ui->uAxisHomeButton->setEnabled(mMotionHandler->uAxisEnabled);

    //step move
    ui->xAxisStepMoveButton->setEnabled(mMotionHandler->xAxisEnabled);
    ui->yAxisStepMoveButton->setEnabled(mMotionHandler->yAxisEnabled);
    ui->zAxisStepMoveButton->setEnabled(mMotionHandler->zAxisEnabled);
    ui->uAxisStepMoveButton->setEnabled(mMotionHandler->uAxisEnabled);

    //position move
    ui->xAxisPositionMoveButton->setEnabled(mMotionHandler->xAxisEnabled);
    ui->yAxisPositionMoveButton->setEnabled(mMotionHandler->yAxisEnabled);
    ui->zAxisPositionMoveButton->setEnabled(mMotionHandler->zAxisEnabled);
    ui->uAxisPositionMoveButton->setEnabled(mMotionHandler->uAxisEnabled);

    return;
}

//******************************************
//motion

//------------------------------------------
//enable joystick free run
//re-assign joystick buttons to free runnning
void Magrathea::enableJoystickFreeRun(bool checked)
{
    if (checked)
    {
        //disconnect signals from slots
        ui->positiveXButton->disconnect();
        ui->negativeXButton->disconnect();
        ui->positiveYButton->disconnect();
        ui->negativeYButton->disconnect();
        ui->positiveZButton->disconnect();
        ui->negativeZButton->disconnect();
        ui->positiveUButton->disconnect();
        ui->negativeUButton->disconnect();

        //connect signals to slots
        //NOTE free run requires parameters
        connect(ui->positiveXButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->negativeXButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->positiveYButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->negativeYButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->positiveZButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->negativeZButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->positiveUButton, SIGNAL(pressed()), this, SLOT(freeRun()));
        connect(ui->negativeUButton, SIGNAL(pressed()), this, SLOT(freeRun()));

        //NOTE end run does *not* require parameters
        connect(ui->positiveXButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunX);
        connect(ui->negativeXButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunX);
        connect(ui->positiveYButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunY);
        connect(ui->negativeYButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunY);
        connect(ui->positiveZButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunZ);
        connect(ui->negativeZButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunZ);
        connect(ui->positiveUButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunU);
        connect(ui->negativeUButton, &QPushButton::released, mMotionHandler, &MotionHandler::endRunU);
    }
}

//------------------------------------------
//free run proxy function to pass arguments to the MotionHandler
void Magrathea::freeRun()
{
    if (sender() == ui->positiveXButton)
        mMotionHandler->runX(+1, ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->runX(-1, ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->runY(+1, ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->runY(-1, ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->runZ(+1, ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->runZ(-1, ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->runU(+1, ui->uAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->runU(-1, ui->uAxisSpeedLineEdit->text().toDouble());
    return;
}

//------------------------------------------
//enable joystick step motion
//re assign joystick buttons to step motion
void Magrathea::enableJoystickStepMotion(bool checked)
{
    if (checked)
    {
        //disconnect signals from slots
        ui->positiveXButton->disconnect();
        ui->negativeXButton->disconnect();
        ui->positiveYButton->disconnect();
        ui->negativeYButton->disconnect();
        ui->positiveZButton->disconnect();
        ui->negativeZButton->disconnect();
        ui->positiveUButton->disconnect();
        ui->negativeUButton->disconnect();

        //connect signals to slots
        //NOTE step motion requires parameters
        connect(ui->positiveXButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->negativeXButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->positiveYButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->negativeYButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->positiveZButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->negativeZButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->positiveUButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));
        connect(ui->negativeUButton, SIGNAL(clicked(bool)), this, SLOT(stepMotion()));

        //set repeat delay
        ui->xAxisStepMoveButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->positiveXButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->negativeXButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->yAxisStepMoveButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->positiveYButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->negativeYButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->zAxisStepMoveButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->positiveZButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->negativeZButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->uAxisStepMoveButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->positiveUButton->setAutoRepeatDelay(autoRepeatDelay);
        ui->negativeUButton->setAutoRepeatDelay(autoRepeatDelay);

        //set repeat interval
        ui->xAxisStepMoveButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->positiveXButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->negativeXButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->yAxisStepMoveButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->positiveYButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->negativeYButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->zAxisStepMoveButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->positiveZButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->negativeZButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->uAxisStepMoveButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->positiveUButton->setAutoRepeatInterval(autoRepeatInterval);
        ui->negativeUButton->setAutoRepeatInterval(autoRepeatInterval);
    }
}

//------------------------------------------
//step motion proxy function to pass arguments to the MotionHandler
void Magrathea::stepMotion()
{
    //joystick
    if (sender() == ui->positiveXButton)
        mMotionHandler->moveXBy(+1*abs(ui->xAxisStepLineEdit->text().toDouble()), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->moveXBy(-1*abs(ui->xAxisStepLineEdit->text().toDouble()), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->moveYBy(+1*abs(ui->yAxisStepLineEdit->text().toDouble()), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->moveYBy(-1*abs(ui->yAxisStepLineEdit->text().toDouble()), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->moveZBy(+1*abs(ui->zAxisStepLineEdit->text().toDouble()), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->moveZBy(-1*abs(ui->zAxisStepLineEdit->text().toDouble()), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->moveUBy(+1*abs(ui->uAxisStepLineEdit->text().toDouble()), ui->uAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->moveUBy(-1*abs(ui->uAxisStepLineEdit->text().toDouble()), ui->uAxisSpeedLineEdit->text().toDouble());

    //naviagtion panel
    else if  (sender() == ui->xAxisStepMoveButton)
        mMotionHandler->moveXBy(ui->xAxisStepLineEdit->text().toDouble(), ui->xAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->yAxisStepMoveButton)
        mMotionHandler->moveYBy(ui->yAxisStepLineEdit->text().toDouble(), ui->yAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->zAxisStepMoveButton)
        mMotionHandler->moveZBy(ui->zAxisStepLineEdit->text().toDouble(), ui->zAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->uAxisStepMoveButton)
        mMotionHandler->moveUBy(ui->uAxisStepLineEdit->text().toDouble(), ui->uAxisSpeedLineEdit->text().toDouble());
    return;
}

//------------------------------------------
//position move proxy function to pass arguments to the MotionHandler
void Magrathea::positionMove()
{
    if (sender() == ui->xAxisPositionMoveButton)
        mMotionHandler->moveXTo(ui->xAxisPositionMoveLine->text().toDouble(), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->yAxisPositionMoveButton)
        mMotionHandler->moveYTo(ui->yAxisPositionMoveLine->text().toDouble(), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->zAxisPositionMoveButton)
        mMotionHandler->moveZTo(ui->zAxisPositionMoveLine->text().toDouble(), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->uAxisPositionMoveButton)
        mMotionHandler->moveUTo(ui->uAxisPositionMoveLine->text().toDouble(), ui->uAxisSpeedLineEdit->text().toDouble());
    return;
}

//------------------------------------------
//step motion autorepeat
void Magrathea::axisStepRepeatBoxClicked(bool checked)
{
    if (sender() == ui->xAxisStepRepeatBox) {
        ui->xAxisStepMoveButton->setAutoRepeat(checked);
        ui->positiveXButton->setAutoRepeat(checked);
        ui->negativeXButton->setAutoRepeat(checked);
    }
    else if (sender() == ui->yAxisStepRepeatBox) {
        ui->yAxisStepMoveButton->setAutoRepeat(checked);
        ui->positiveYButton->setAutoRepeat(checked);
        ui->negativeYButton->setAutoRepeat(checked);
    }
    else if (sender() == ui->zAxisStepRepeatBox) {
        ui->zAxisStepMoveButton->setAutoRepeat(checked);
        ui->positiveZButton->setAutoRepeat(checked);
        ui->negativeZButton->setAutoRepeat(checked);
    }
    else if (sender() == ui->uAxisStepRepeatBox) {
        ui->uAxisStepMoveButton->setAutoRepeat(checked);
        ui->positiveUButton->setAutoRepeat(checked);
        ui->negativeUButton->setAutoRepeat(checked);
    }
    return;
}
