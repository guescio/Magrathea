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
#include <QSignalMapper>

//******************************************
Magrathea::Magrathea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Magrathea),
    mMotionHandler(new MotionHandler)
{
    ui->setupUi(this);

    //------------------------------------------
    //variables
    autoRepeatDelay=1000;//ms

    //------------------------------------------
    //output log
    outputLogTextEdit = new QTextEdit;
    outputLogTextEdit->setReadOnly(true);
    ui->leftTabWidget->addTab(outputLogTextEdit, "log");
    ui->leftTabWidget->setCurrentWidget(outputLogTextEdit);

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
    ui->enableAxesBox->setEnabled(false);
    ui->freeRunRadioButton->setChecked(true);

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
    connect(ui->enableCameraBox,
            SIGNAL(toggled(bool)),
            this,
            SLOT(enableCameraBoxToggled(bool)));

    connect(ui->focusButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(focusButtonClicked()));

    connect(ui->captureButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(captureButtonClicked()));

    //gantry
    connect(ui->connectGantryBox,
            SIGNAL(clicked(bool)),
            this,
            SLOT(connectGantryBoxClicked(bool)));

    connect(ui->enableAxesBox,
            SIGNAL(clicked(bool)),
            this,
            SLOT(enableAxesBoxClicked(bool)));

    connect(ui->homeAxesButton,
            &QPushButton::clicked,
            mMotionHandler,
            &MotionHandler::Home);

    //joystick
    //free run or step motion
    connect(ui->freeRunRadioButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(enableJoystickFreeRun(bool)));
    connect(ui->stepRadioButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(enableJoystickStepMotion(bool)));
    connect(ui->xAxisStepContinousBox,
            SIGNAL(clicked(bool)),
            this,
            SLOT(xAxisStepContinousBoxToggled(bool)));

    //y axis
    connect(ui->positiveYButton,
            SIGNAL(pressed()),
            this,
            SLOT(positiveYButtonClicked()));
    ui->positiveYButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->negativeYButton,
            SIGNAL(pressed()),
            this,
            SLOT(negativeYButtonClicked()));
    ui->negativeYButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->yAxisStepContinousBox,
            SIGNAL(toggled(bool)),
            this,
            SLOT(yAxisStepContinousBoxToggled(bool)));

    //z axis
    connect(ui->positiveZButton,
            SIGNAL(pressed()),
            this,
            SLOT(positiveZButtonClicked()));
    ui->positiveZButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->negativeZButton,
            SIGNAL(pressed()),
            this,
            SLOT(negativeZButtonClicked()));
    ui->negativeZButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->zAxisStepContinousBox,
            SIGNAL(toggled(bool)),
            this,
            SLOT(zAxisStepContinousBoxToggled(bool)));
    //u axis
    connect(ui->positiveUButton,
            SIGNAL(pressed()),
            this,
            SLOT(positiveUButtonClicked()));
    ui->positiveUButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->negativeUButton,
            SIGNAL(pressed()),
            this,
            SLOT(negativeUButtonClicked()));
    ui->negativeUButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->uAxisStepContinousBox,
            SIGNAL(toggled(bool)),
            this,
            SLOT(uAxisStepContinousBoxToggled(bool)));
}

//******************************************
Magrathea::~Magrathea()
{
    delete ui;
    delete mMotionHandler;
}

//******************************************
//x axis
void Magrathea::enableJoystickFreeRun(bool checked)
{
    if (checked)
    {
        //disconnect signal from all slots
        ui->positiveXButton->disconnect();
        //NOTE free run requires parameters
        connect(ui->positiveXButton,
                SIGNAL(pressed()),
                this,
                SLOT(freeRun()));
        //NOTE end run does *not* require parameters
        connect(ui->positiveXButton,
                &QPushButton::released,
                mMotionHandler,
                &MotionHandler::EndRunX);
    }
}

//******************************************
void Magrathea::enableJoystickStepMotion(bool checked)
{
    if (checked)
    {
        ui->positiveXButton->disconnect();
        //NOTE step motion requires parameters
        connect(ui->positiveXButton,
                SIGNAL(clicked(bool)),
                this,
                SLOT(stepMotion()));
        ui->positiveXButton->setAutoRepeatDelay(autoRepeatDelay);
    }
}

//******************************************
//enable camera
void Magrathea::enableCameraBoxToggled(bool toggled)
{
    if (toggled) mCamera->start();
    else mCamera->stop();
    return;
}

//******************************************
//camera focus
void Magrathea::focusButtonClicked()
{
    qInfo("camera focus");
    return;
}

//******************************************
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
void Magrathea::connectGantryBoxClicked(bool clicked)
{
    if (clicked)
    {
        if (mMotionHandler->ConnectGantry()) {
            ui->enableAxesBox->setEnabled(true);
        } else {
            ui->connectGantryBox->setChecked(false);
            qWarning("could not connect to gantry");
        }
    } else {
        if (mMotionHandler->axesEnabled) {
            ui->connectGantryBox->setChecked(true);
            qWarning("disable axes before disconnecting from gantry");
        } else {
            if(mMotionHandler->DisconnectGantry())
                ui->enableAxesBox->setEnabled(false);
            else {
                ui->connectGantryBox->setChecked(true);
                qWarning("could not disconnect from gantry");
            }
        }
    }
}

//------------------------------------------
//enable axes
//proxy function to handle the EnableAxes and DisableAxes functions from MotionHandler
void Magrathea::enableAxesBoxClicked(bool clicked)
{
    if (clicked)
    {
        if (mMotionHandler->gantryConnected) {
            if (mMotionHandler->EnableAxes()) {
                ui->connectGantryBox->setEnabled(false);
            } else {
                ui->enableAxesBox->setChecked(false);
                qWarning("could not enable axes");
            }
        } else {
            ui->enableAxesBox->setChecked(false);
            qWarning("connect to the gantry before enabling axes");
        }
    } else {
        if (mMotionHandler->DisableAxes()) {
            ui->enableAxesBox->setChecked(false);
            ui->connectGantryBox->setEnabled(true);
        } else {
            ui->enableAxesBox->setCheckable(true);
            qWarning("could not disable axes");
        }
    }
}

//******************************************
//x axis
void Magrathea::freeRun()
{
    if (sender() == ui->positiveXButton)
        mMotionHandler->RunX(1, ui->xAxisSpeedLineEdit->text().toDouble());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->RunX(-1, ui->xAxisSpeedLineEdit->text().toDouble());
    return;
}

void Magrathea::stepMotion()
{
    if (sender() == ui->positiveXButton)
        mMotionHandler->MoveXBy(ui->xAxisStepLineEdit->text().toDouble(),
                                ui->xAxisSpeedLineEdit->text().toDouble());
    return;
}

void Magrathea::xAxisStepContinousBoxToggled(bool flag)
{
    ui->positiveXButton->setAutoRepeat(flag);
    ui->negativeXButton->setAutoRepeat(flag);
    return;
}

//******************************************
//y axis
void Magrathea::positiveYButtonClicked()
{
    qInfo("move forward");
    return;
}

void Magrathea::negativeYButtonClicked()
{
    qInfo("move backward");
    return;
}

void Magrathea::yAxisStepContinousBoxToggled(bool flag)
{
    ui->positiveYButton->setAutoRepeat(flag);
    ui->negativeYButton->setAutoRepeat(flag);
    return;
}

//******************************************
//z axis
void Magrathea::positiveZButtonClicked()
{
    qInfo("move up");
    return;
}

void Magrathea::negativeZButtonClicked()
{
    qInfo("move down");
    return;
}

void Magrathea::zAxisStepContinousBoxToggled(bool flag)
{
    ui->positiveZButton->setAutoRepeat(flag);
    ui->negativeZButton->setAutoRepeat(flag);
    return;
}

//******************************************
//u axis
void Magrathea::positiveUButtonClicked()
{
    qInfo("move closkwise");
    return;
}

void Magrathea::negativeUButtonClicked()
{
    qInfo("move counterclockwise");
    return;
}

void Magrathea::uAxisStepContinousBoxToggled(bool flag)
{
    ui->positiveUButton->setAutoRepeat(flag);
    ui->negativeUButton->setAutoRepeat(flag);
    return;
}
