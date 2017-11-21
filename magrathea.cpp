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

//******************************************
Magrathea::Magrathea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Magrathea),
    mMotionHandler(new MotionHandler)
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
        qInfo("in Vancouver with Aerotech A3200 gantry");
    #else
        qInfo("code development with no gantry");
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
    enableThings(false);
    //ui->freeRunRadioButton->setChecked(true);

    //------------------------------------------
    //position
    ui->xAxisPositionLine2->setReadOnly(true);
    ui->yAxisPositionLine2->setReadOnly(true);
    ui->zAxisPositionLine2->setReadOnly(true);
    ui->uAxisPositionLine2->setReadOnly(true);

    //------------------------------------------
    //navigation
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

    //joystick
    connect(ui->freeRunRadioButton, SIGNAL(clicked(bool)), this, SLOT(enableJoystickFreeRun(bool)));
    connect(ui->stepRadioButton,    SIGNAL(clicked(bool)), this, SLOT(enableJoystickStepMotion(bool)));

    //home axes
    connect(ui->homeAxesButton,  &QPushButton::clicked, mMotionHandler, &MotionHandler::Home);
    connect(ui->xAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::HomeX);
    connect(ui->yAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::HomeY);
    connect(ui->zAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::HomeZ);
    connect(ui->uAxisHomeButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::HomeU);

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
        if (mMotionHandler->ConnectGantry()) {
            ui->enableAxesButton->setEnabled(true);
            ui->disableAxesButton->setEnabled(true);
            ui->xAxisEnableBox->setEnabled(true);
            ui->yAxisEnableBox->setEnabled(true);
            ui->zAxisEnableBox->setEnabled(true);
            ui->uAxisEnableBox->setEnabled(true);
        } else {
            ui->connectGantryBox->setChecked(false);
            qWarning("could not connect to gantry");
        }
    } else {
        if (mMotionHandler->xAxisEnabled || mMotionHandler->yAxisEnabled || mMotionHandler->zAxisEnabled || mMotionHandler->uAxisEnabled) {
            ui->connectGantryBox->setChecked(true);
            qWarning("disable axes before disconnecting from gantry");
        } else {
            if(mMotionHandler->DisconnectGantry()) {
                ui->enableAxesButton->setEnabled(false);
                ui->disableAxesButton->setEnabled(false);
                ui->xAxisEnableBox->setEnabled(false);
                ui->yAxisEnableBox->setEnabled(false);
                ui->zAxisEnableBox->setEnabled(false);
                ui->uAxisEnableBox->setEnabled(false);
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
    } else if (sender() == ui->xAxisEnableBox) mMotionHandler->EnableXAxis(checked);
      else if (sender() == ui->yAxisEnableBox) mMotionHandler->EnableYAxis(checked);
      else if (sender() == ui->zAxisEnableBox) mMotionHandler->EnableZAxis(checked);
      else if (sender() == ui->uAxisEnableBox) mMotionHandler->EnableUAxis(checked);

    //check enabled axes
    if (mMotionHandler->xAxisEnabled || mMotionHandler->yAxisEnabled || mMotionHandler->zAxisEnabled || mMotionHandler->uAxisEnabled) {
        ui->connectGantryBox->setEnabled(false);
        enableThings(true);
    } else {
        ui->connectGantryBox->setEnabled(true);
        enableThings(false);
    }

    return;
}

//------------------------------------------
//once the gantry is connected and the axes are enables, enable things
void Magrathea::enableThings(bool checked) {

    //joystick
    ui->leftTabWidget->widget(0)->setEnabled(checked);

    //motion
    ui->xAxisHomeButton->setEnabled(checked);
    ui->yAxisHomeButton->setEnabled(checked);
    ui->zAxisHomeButton->setEnabled(checked);
    ui->uAxisHomeButton->setEnabled(checked);

    ui->xAxisStepMoveButton->setEnabled(checked);
    ui->yAxisStepMoveButton->setEnabled(checked);
    ui->zAxisStepMoveButton->setEnabled(checked);
    ui->uAxisStepMoveButton->setEnabled(checked);

    ui->xAxisPositionMoveButton->setEnabled(checked);
    ui->yAxisPositionMoveButton->setEnabled(checked);
    ui->zAxisPositionMoveButton->setEnabled(checked);
    ui->uAxisPositionMoveButton->setEnabled(checked);

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
        connect(ui->positiveXButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunX);
        connect(ui->negativeXButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunX);
        connect(ui->positiveYButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunY);
        connect(ui->negativeYButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunY);
        connect(ui->positiveZButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunZ);
        connect(ui->negativeZButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunZ);
        connect(ui->positiveUButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunU);
        connect(ui->negativeUButton, &QPushButton::released, mMotionHandler, &MotionHandler::EndRunU);
    }
}

//------------------------------------------
//free run proxy function to pass arguments to the MotionHandler
void Magrathea::freeRun()
{
    if (sender() == ui->positiveXButton)
        mMotionHandler->RunX(+1, ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->RunX(-1, ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->RunY(+1, ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->RunY(-1, ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->RunZ(+1, ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->RunZ(-1, ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->RunU(+1, ui->uAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->RunU(-1, ui->uAxisSpeedLineEdit->text().toDouble());
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
        mMotionHandler->MoveXBy(+1*abs(ui->xAxisStepLineEdit->text().toDouble()), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->MoveXBy(-1*abs(ui->xAxisStepLineEdit->text().toDouble()), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->MoveYBy(+1*abs(ui->yAxisStepLineEdit->text().toDouble()), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->MoveYBy(-1*abs(ui->yAxisStepLineEdit->text().toDouble()), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->MoveZBy(+1*abs(ui->zAxisStepLineEdit->text().toDouble()), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->MoveZBy(-1*abs(ui->zAxisStepLineEdit->text().toDouble()), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->MoveUBy(+1*abs(ui->uAxisStepLineEdit->text().toDouble()), ui->uAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->MoveUBy(-1*abs(ui->uAxisStepLineEdit->text().toDouble()), ui->uAxisSpeedLineEdit->text().toDouble());

    //naviagtion panel
    else if  (sender() == ui->xAxisStepMoveButton)
        mMotionHandler->MoveXBy(ui->xAxisStepLineEdit->text().toDouble(), ui->xAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->yAxisStepMoveButton)
        mMotionHandler->MoveYBy(ui->yAxisStepLineEdit->text().toDouble(), ui->yAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->zAxisStepMoveButton)
        mMotionHandler->MoveZBy(ui->zAxisStepLineEdit->text().toDouble(), ui->zAxisSpeedLineEdit->text().toDouble());
    else if  (sender() == ui->uAxisStepMoveButton)
        mMotionHandler->MoveUBy(ui->uAxisStepLineEdit->text().toDouble(), ui->uAxisSpeedLineEdit->text().toDouble());
    return;
}

//------------------------------------------
//position move proxy function to pass arguments to the MotionHandler
void Magrathea::positionMove()
{
    if (sender() == ui->xAxisPositionMoveButton)
        mMotionHandler->MoveXTo(ui->xAxisPositionMoveLine->text().toDouble(), ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->yAxisPositionMoveButton)
        mMotionHandler->MoveYTo(ui->yAxisPositionMoveLine->text().toDouble(), ui->yAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->zAxisPositionMoveButton)
        mMotionHandler->MoveZTo(ui->zAxisPositionMoveLine->text().toDouble(), ui->zAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->uAxisPositionMoveButton)
        mMotionHandler->MoveUTo(ui->uAxisPositionMoveLine->text().toDouble(), ui->uAxisSpeedLineEdit->text().toDouble());
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
