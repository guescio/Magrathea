#include "magrathea.h"
#include "ui_magrathea.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraFocus>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QtMessageHandler>
#include <MotionHandler.h>
#include <cmath>
#ifdef VANCOUVER

#include <Gantry_Vancouver.h>

#endif
#include <PetalAlgManager.h>
#include <gluehandler.h>

int hold = 0; // used for drop-down menus

//******************************************
Magrathea::Magrathea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Magrathea)
{
    ui->setupUi(this);

    //------------------------------------------
    //variables
    autoRepeatDelay=1000;
    mStepDefault = 5;
    mStepSmall = 1;
    mStepTiny = 0.5;
    mStep = mStepDefault;

    //------------------------------------------
    //output log
    outputLogTextEdit = new QTextEdit;
    outputLogTextEdit->setReadOnly(true);
    ui->leftTabWidget->addTab(outputLogTextEdit, "log");
    ui->leftTabWidget->setCurrentWidget(outputLogTextEdit);

    //------------------------------------------
    //gantry
#ifdef VANCOUVER
    mMotionHandler = new Gantry_Vancouver();
#else
    qInfo("You didn't tell me where your gantry is!");
    return;
#endif

    //------------------------------------------
    // glue
    // mGlueHandler = new GlueHandler();

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
    connect(ui->connectJoystickButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(connectJoystickBoxToggled(bool)));

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

    //------------------------------------------
    // connect directions for specific
    // algorithms

    // Algorithm handler
    mPetalManager = new PetalAlgManager(mMotionHandler);

    connect(ui->InnerFiducialButton,
            SIGNAL(pressed()),
            this,
            SLOT(InnerFiducialButtonPressed()));
    connect(ui->InnerFoundButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(InnerFoundButtonToggled(bool)));
    connect(ui->OuterFiducialButton,
            SIGNAL(pressed()),
            this,
            SLOT(OuterFiducialButtonPressed()));
    connect(ui->OuterFoundButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(OuterFoundButtonToggled(bool)));

    connect(ui->SensorPullDown,
            SIGNAL(activated(int)),
            this,
            SLOT(SensorPullDownActivated(int)));
    connect(ui->SensorFirstFoundButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(SensorFirstFoundButtonToggled(bool)));
    connect(ui->SensorSecondFoundButton,
            SIGNAL(toggled(bool)),
            this,
            SLOT(SensorSecondFoundButtonToggled(bool)));
    connect(ui->PickUpButton,
            SIGNAL(pressed()),
            this,
            SLOT(PickUpButtonPressed()));

    connect(ui->PetalPullDown,
            SIGNAL(activated(int)),
            this,
            SLOT(PetalPullDownActivated(int)));
    connect(ui->FiducialPullDown,
            SIGNAL(activated(int)),
            this,
            SLOT(FiducialPullDownActivated(int)));
    connect(ui->ReviewButton,
            SIGNAL(pressed()),
            this,
            SLOT(ReviewButtonPressed()));
    connect(ui->PreviousFidButton,
            SIGNAL(pressed()),
            this,
            SLOT(PreviousFidButtonPressed()));
    connect(ui->NextFidButton,
            SIGNAL(pressed()),
            this,
            SLOT(NextFidButtonPressed()));
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
// connect joystick
void Magrathea::connectJoystickBoxToggled(bool toggled)
{
    if (toggled) {
        mMotionHandler->ConnectJoystick();
    } else {
        mMotionHandler->DisconnectJoystick();
    }
    return;
}

//******************************************
//x axis
void Magrathea::positiveXButtonClicked()
{
    mMotionHandler->MoveBy(mStep,0,0,0);
    return;
}

void Magrathea::negativeXButtonClicked()
{
    mMotionHandler->MoveBy(-1*mStep,0,0,0);
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
    mMotionHandler->MoveBy(0,mStep,0,0);
    return;
}

void Magrathea::negativeYButtonClicked()
{
    mMotionHandler->MoveBy(0,-1*mStep,0,0);
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
    mMotionHandler->MoveBy(0,0,mStep,0);
    return;
}

void Magrathea::negativeZButtonClicked()
{
    mMotionHandler->MoveBy(0,0,-1*mStep,0);
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
    mMotionHandler->MoveBy(0,0,0,mStep);
    return;
}

void Magrathea::negativeUButtonClicked()
{
    mMotionHandler->MoveBy(0,0,0,-1*mStep);
    return;
}

void Magrathea::uAxisStepContinousBoxToggled(bool flag)
{
    ui->positiveUButton->setAutoRepeat(flag);
    ui->negativeUButton->setAutoRepeat(flag);
    return;
}

//******************************************
// algorithms:
// Pick up and place
void Magrathea::InnerFiducialButtonPressed()
{
    // Go to the inner fiducial location
    mPetalManager->InnerFiducial();

    return;
}

void Magrathea::InnerFoundButtonToggled(bool flag)
{
    if(flag == true)
    {
        // Store coordinates
        mPetalManager->StoreInner();

        if((ui->OuterFoundButton->isChecked() == true) && (ui->SensorSecondFoundButton->isChecked() == true))
                ui->PickUpButton->setEnabled(true);

        if((ui->OuterFoundButton->isChecked() == true) && (ui->FiducialPullDown->currentIndex() > 0))
                ui->ReviewButton->setEnabled(true);
    }
    else // flag == false
    {
        ui->PickUpButton->setEnabled(false);
        ui->ReviewButton->setEnabled(false);
        ui->NextFidButton->setEnabled(false);
        ui->PreviousFidButton->setEnabled(false);
    }

    return;
}

void Magrathea::OuterFiducialButtonPressed()
{
    // Go to the outer fiducial location
    mPetalManager->OuterFiducial();

    return;
}

void Magrathea::OuterFoundButtonToggled(bool flag)
{
    if(flag == true)
    {
        // Store coordinates
        mPetalManager->StoreOuter();

        if((ui->InnerFoundButton->isChecked() == true) && (ui->SensorSecondFoundButton->isChecked() == true))
                ui->PickUpButton->setEnabled(true);

        if((ui->InnerFoundButton->isChecked() == true) && (ui->FiducialPullDown->currentIndex() > 0))
                ui->ReviewButton->setEnabled(true);
    }
    else // flag == false
    {
        ui->PickUpButton->setEnabled(false);
        ui->ReviewButton->setEnabled(false);
        ui->NextFidButton->setEnabled(false);
        ui->PreviousFidButton->setEnabled(false);
    }

    return;
}

void Magrathea::SensorPullDownActivated(int index)
{
    std::string sensor = "R0";

    if(index == 0)
        sensor = "R0";
    if(index == 1)
        sensor = "R1";
    if(index == 2)
        sensor = "R2";
    if(index == 3)
        sensor = "R3-Top";
    if(index == 4)
        sensor = "R3-Bottom";
    if(index == 5)
        sensor = "R4-Top";
    if(index == 6)
        sensor = "R4-Bottom";
    if(index == 7)
        sensor = "R5-Top";
    if(index == 8)
        sensor = "R5-Bottom";

    // Go to chosen sensor
    mPetalManager->whichSensor(sensor, 1);

    if(index > 0)
        ui->SensorFirstFoundButton->setEnabled(true);
    else
    {
        ui->SensorFirstFoundButton->setChecked(false);
        ui->SensorFirstFoundButton->setEnabled(false);
    }

    if(index != hold)
    {
        ui->SensorFirstFoundButton->setChecked(false);
        ui->SensorSecondFoundButton->setChecked(false);
        ui->SensorSecondFoundButton->setEnabled(false);
    }

    hold = index;
    return;
}

void Magrathea::SensorFirstFoundButtonToggled(bool flag)
{
    if(flag == true)
    {
        // Store coordinates
        mPetalManager->StoreFirst();
        ui->SensorSecondFoundButton->setEnabled(true);
    }
    else // flag == false
    {
        ui->SensorSecondFoundButton->setChecked(false);
        ui->SensorSecondFoundButton->setEnabled(false);
    }

    return;
}

void Magrathea::SensorSecondFoundButtonToggled(bool flag)
{
    if(ui->SensorFirstFoundButton->isChecked() == true)
    {
        if(flag == true)
        {
            // Store coordinates
            mPetalManager->StoreSecond();

            if((ui->InnerFoundButton->isChecked() == true) && (ui->OuterFoundButton->isChecked() == true))
                ui->PickUpButton->setEnabled(true);
        }
        else // flag == false
        {
            ui->PickUpButton->setEnabled(false);
        }
    }
    else // first checkbox not checked
    {
        ui->SensorSecondFoundButton->setChecked(false);
        ui->PickUpButton->setEnabled(false);
    }

    return;
}

void Magrathea::PickUpButtonPressed()
{
    mPetalManager->PickUp();

    return;
}

void Magrathea::PetalPullDownActivated(int index)
{
    std::string sensor = "R0";

    if(index == 1)
        sensor = "R0";
    if(index == 2)
        sensor = "R1";
    if(index == 3)
        sensor = "R2";
    if(index == 4)
        sensor = "R3-Top";
    if(index == 5)
        sensor = "R3-Bottom";
    if(index == 6)
        sensor = "R4-Top";
    if(index == 7)
        sensor = "R4-Bottom";
    if(index == 8)
        sensor = "R5-Top";
    if(index == 9)
        sensor = "R5-Bottom";

    // Go to chosen sensor
    mPetalManager->whichSensor(sensor, 0);

    if(index > 0)
        ui->FiducialPullDown->setEnabled(true);
    else
        ui->FiducialPullDown->setEnabled(false);

    if(index != hold)
    {
        ui->PreviousFidButton->setEnabled(false);
        ui->NextFidButton->setEnabled(false);
        ui->ReviewButton->setEnabled(false);
        ui->FiducialPullDown->setCurrentIndex(0);
    }

    hold = index;
    return;
}

void Magrathea::FiducialPullDownActivated(int index)
{
    std::string fiducial = "Top-Left";

    if(index == 1)
        fiducial = "Top-Left";
    if(index == 2)
        fiducial = "Top-Right";
    if(index == 3)
        fiducial = "Bottom-Left";
    if(index == 4)
        fiducial = "Bottom-Right";

    // Go to chosen fiducial
    //mPetalManager->whichSensor(sensor);

    if(index > 0)
        ui->ReviewButton->setEnabled(true);
    else
        ui->ReviewButton->setEnabled(false);

    return;
}

void Magrathea::ReviewButtonPressed()
{
    // Go to fiducial
    mPetalManager->review();

    ui->PreviousFidButton->setEnabled(true);
    ui->NextFidButton->setEnabled(true);

    return;
}

void Magrathea::PreviousFidButtonPressed()
{
    return;
}

void Magrathea::NextFidButtonPressed()
{
    return;
}

// Glue
void Magrathea::glueButtonClicked()
{

    //mPetalManager->SetGlueDispenser(mGlueHandler);
    //mPetalManager->PutSomeGlueDots();
    return;

}
