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
    //gantry
    //MotionHandler gantry;

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
            &QCheckBox::toggled,
            mMotionHandler,
            &MotionHandler::ActivateGantry);

    connect(ui->enableAxesBox,
            &QCheckBox::toggled,
            mMotionHandler,
            &MotionHandler::ActivateAllAxes);

    connect(ui->homeAxesButton,
            &QCheckBox::clicked,
            mMotionHandler,
            &MotionHandler::Home);

    //joystick buttons
    //x axis
    connect(ui->positiveXButton,
            SIGNAL(pressed()),
            this,
            SLOT(positiveXButtonClicked()));
    ui->positiveXButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->negativeXButton,
            SIGNAL(pressed()),
            this,
            SLOT(negativeXButtonClicked()));
    ui->negativeXButton->setAutoRepeatDelay(autoRepeatDelay);

    connect(ui->xAxisStepContinousBox,
            SIGNAL(toggled(bool)),
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
//x axis
void Magrathea::positiveXButtonClicked()
{
    qInfo("move right");
    return;
}

void Magrathea::negativeXButtonClicked()
{
    qInfo("move left");
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
