#include "magrathea.h"
#include "ui_magrathea.h"
#include <QTimer>
#include <QFont>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QtMessageHandler>
#include <MotionHandler.h>
#include <cmath>
#include <QMessageBox>
#include "calibrator.h"
#ifdef VANCOUVER
#include <AerotechMotionhandler.h>
#elif VALENCIA
#include <ACSCMotionHandler.h>
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
    mMotionHandler = new AerotechMotionHandler();
#elif VALENCIA
    qInfo("Valencia, ACSC gantry");
    mMotionHandler = new ACSCMotionHandler();
#else
#define DEBUG
    qInfo("where is your gantry?");
    mMotionHandler = new MotionHandler();
#endif

    //------------------------------------------
    //font
    //QFont font=ui->pushButton->property("font").value<QFont>();
    //qDebug()<<font.family()<<font.pointSize();
    QFont font("");
    font.setStyleHint(QFont::Monospace);

    //position
    ui->xAxisPositionLine->setFont(font);
    ui->yAxisPositionLine->setFont(font);
    ui->zAxisPositionLine->setFont(font);
    ui->uAxisPositionLine->setFont(font);

    ui->xAxisPositionLine2->setFont(font);
    ui->yAxisPositionLine2->setFont(font);
    ui->zAxisPositionLine2->setFont(font);
    ui->uAxisPositionLine2->setFont(font);

    //step move
    ui->xAxisStepDoubleSpinBox->setFont(font);
    ui->xAxisStepDoubleSpinBox->setValue(10.0);
    ui->xAxisStepDoubleSpinBox->setMinimum(-1000.0);
    ui->xAxisStepDoubleSpinBox->setMaximum(1000.0);
    ui->xAxisStepDoubleSpinBox->setDecimals(3);
    ui->xAxisStepDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->yAxisStepDoubleSpinBox->setFont(font);
    ui->yAxisStepDoubleSpinBox->setValue(10.0);
    ui->yAxisStepDoubleSpinBox->setMinimum(-1000.0);
    ui->yAxisStepDoubleSpinBox->setMaximum(1000.0);
    ui->yAxisStepDoubleSpinBox->setDecimals(3);
    ui->yAxisStepDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->zAxisStepDoubleSpinBox->setFont(font);
    ui->zAxisStepDoubleSpinBox->setValue(10.0);
    ui->zAxisStepDoubleSpinBox->setMinimum(-300.0);
    ui->zAxisStepDoubleSpinBox->setMaximum(300.0);
    ui->zAxisStepDoubleSpinBox->setDecimals(3);
    ui->zAxisStepDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->uAxisStepDoubleSpinBox->setFont(font);
    ui->uAxisStepDoubleSpinBox->setValue(10.0);
    ui->uAxisStepDoubleSpinBox->setDecimals(3);
    ui->uAxisStepDoubleSpinBox->setAlignment(Qt::AlignRight);

    //position move
    ui->xAxisPositionMoveDoubleSpinBox->setFont(font);
    ui->xAxisPositionMoveDoubleSpinBox->setValue(0.0);
    ui->xAxisPositionMoveDoubleSpinBox->setMinimum(0.0);
    ui->xAxisPositionMoveDoubleSpinBox->setMaximum(1000.0);
    ui->xAxisPositionMoveDoubleSpinBox->setDecimals(3);
    ui->xAxisPositionMoveDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->yAxisPositionMoveDoubleSpinBox->setFont(font);
    ui->yAxisPositionMoveDoubleSpinBox->setValue(0.0);
    ui->yAxisPositionMoveDoubleSpinBox->setMinimum(-1000.0);
    ui->yAxisPositionMoveDoubleSpinBox->setMaximum(0.0);
    ui->yAxisPositionMoveDoubleSpinBox->setDecimals(3);
    ui->yAxisPositionMoveDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->zAxisPositionMoveDoubleSpinBox->setFont(font);
    ui->zAxisPositionMoveDoubleSpinBox->setValue(0.0);
    ui->zAxisPositionMoveDoubleSpinBox->setMinimum(-300.0);
    ui->zAxisPositionMoveDoubleSpinBox->setMaximum(0.0);
    ui->zAxisPositionMoveDoubleSpinBox->setDecimals(3);
    ui->zAxisPositionMoveDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->uAxisPositionMoveDoubleSpinBox->setFont(font);
    ui->uAxisPositionMoveDoubleSpinBox->setValue(0.0);
    ui->uAxisPositionMoveDoubleSpinBox->setMinimum(0.0);
    ui->uAxisPositionMoveDoubleSpinBox->setMaximum(360.0);
    ui->uAxisPositionMoveDoubleSpinBox->setDecimals(3);
    ui->uAxisPositionMoveDoubleSpinBox->setAlignment(Qt::AlignRight);

    //speed
    ui->xAxisSpeedDoubleSpinBox->setFont(font);
    ui->xAxisSpeedDoubleSpinBox->setValue(50.0);
    ui->xAxisSpeedDoubleSpinBox->setMinimum(0.0);
    ui->xAxisSpeedDoubleSpinBox->setMaximum(300.0);
    ui->xAxisSpeedDoubleSpinBox->setDecimals(1);
    ui->xAxisSpeedDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->yAxisSpeedDoubleSpinBox->setFont(font);
    ui->yAxisSpeedDoubleSpinBox->setValue(50.0);
    ui->yAxisSpeedDoubleSpinBox->setMinimum(0.0);
    ui->yAxisSpeedDoubleSpinBox->setMaximum(300.0);
    ui->yAxisSpeedDoubleSpinBox->setDecimals(1);
    ui->yAxisSpeedDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->zAxisSpeedDoubleSpinBox->setFont(font);
    ui->zAxisSpeedDoubleSpinBox->setValue(30.0);
    ui->zAxisSpeedDoubleSpinBox->setMinimum(0.0);
    ui->zAxisSpeedDoubleSpinBox->setMaximum(300.0);
    ui->zAxisSpeedDoubleSpinBox->setDecimals(1);
    ui->zAxisSpeedDoubleSpinBox->setAlignment(Qt::AlignRight);
    ui->uAxisSpeedDoubleSpinBox->setFont(font);
    ui->uAxisSpeedDoubleSpinBox->setValue(30.0);
    ui->uAxisSpeedDoubleSpinBox->setMinimum(0.0);
    ui->uAxisSpeedDoubleSpinBox->setMaximum(300.0);
    ui->uAxisSpeedDoubleSpinBox->setDecimals(1);
    ui->uAxisSpeedDoubleSpinBox->setAlignment(Qt::AlignRight);

    //------------------------------------------
    //timer
    mPositionTimer = new QTimer(this);
    connect(mPositionTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    mPositionTimer->start(100);//ms

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
    connect(ui->Calib_button,    SIGNAL(clicked(bool)), this, SLOT(Calibration_ButtonClicked()));
    connect(ui->Calib_button_2,  SIGNAL(clicked(bool)), this, SLOT(Calibration_2_ButtonClicked()));

    //gantry
    connect(ui->connectGantryBox, SIGNAL(toggled(bool)), this, SLOT(connectGantryBoxClicked(bool)));
    connect(ui->enableAxesButton, SIGNAL(clicked(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->disableAxesButton,SIGNAL(clicked(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->xAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->yAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->zAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->uAxisEnableBox,   SIGNAL(toggled(bool)), this, SLOT(enableAxesClicked(bool)));
    connect(ui->resetErrorButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::acknowledgeMotionFaultGantry);
    connect(ui->stopButton, &QPushButton::clicked, mMotionHandler, &MotionHandler::stop);

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
//timer

//position update
void Magrathea::updatePosition(){
    ui->xAxisPositionLine->setText(QString::number(mMotionHandler->whereAmI()[0], 'f', 3));
    ui->yAxisPositionLine->setText(QString::number(mMotionHandler->whereAmI()[1], 'f', 3));
    ui->zAxisPositionLine->setText(QString::number(mMotionHandler->whereAmI()[2], 'f', 3));
    ui->uAxisPositionLine->setText(QString::number(mMotionHandler->whereAmI()[3], 'f', 3));
    ui->xAxisPositionLine2->setText(QString::number(mMotionHandler->whereAmI()[0], 'f', 3));
    ui->yAxisPositionLine2->setText(QString::number(mMotionHandler->whereAmI()[1], 'f', 3));
    ui->zAxisPositionLine2->setText(QString::number(mMotionHandler->whereAmI()[2], 'f', 3));
    ui->uAxisPositionLine2->setText(QString::number(mMotionHandler->whereAmI()[3], 'f', 3));
    return;
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
    auto filename = QFileDialog::getSaveFileName(this, "capture", "/ ", "image (*.jpg;*.jpeg)");
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
//------------------------------------------
//calibrate

void Magrathea::Calibration_ButtonClicked()
{    calibrationCaller(0); }

void Magrathea::Calibration_2_ButtonClicked()
{    calibrationCaller(1); }

void Magrathea::calibrationCaller(int input){

    Calibrator * calibrator = new Calibrator(this);
    //Eventually add conmmand to move the gantry to the place where the
    // calibration area is.
    mCamera->stop(); //closing QCamera
    if(!cap.open(0))     //Opening opencv-camera, needed for easier image manipulation
        QMessageBox::critical(this, tr("Error"), tr("Could not open camera"));
    std::string Images[12] = {"C:/Users/Silicio/WORK/Full_Size/Mini/0018.bmp",//0
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/image_000_600_60_15.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/image_000_600_60_15_rotated_5_degrees.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/image_000_600_60_15_rotated_30_degrees.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/image_000_600_60_15_rotated_45_degrees.png",//4
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/image_000_600_60_15_rotated_stereo.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/perfect_strips_0_degrees.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/perfect_strips_1.49_degrees.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/perfect_strips_5_degrees.png",
                              "C:/Users/Silicio/WORK/Temporary_files/BNL_images/perfect_strips_45_degrees.png",//9
                              "C:/Users/Silicio/WORK/Temporary_files/image_007_600_60_15_dan.png",
                              "C:/Users/Silicio/WORK/Temporary_files/image_007_600_60_15_dan_rot_min20.png"
                             };
    calibrator->SetImage(Images[3]
                ,CV_LOAD_IMAGE_COLOR);
    calibrator->Set_camera(cap);
    calibrator->Set_log(outputLogTextEdit);
    double calibration_value     = -100;
    double calibration_value_err = -10;
    bool is_px_over_micron = (input == 0);
    calibrator->Calibration_strips(calibration_value,calibration_value_err, is_px_over_micron);
    QString unit = (is_px_over_micron ? " px/um" : " um/px");
    QString output = "C: "+QString::number(calibration_value)+ " +- "+QString::number(calibration_value_err)+ unit;
    ui->Calib_value_lineEdit->setText(output);
    delete calibrator;
    cap.release();         //Going back to QCameraa
    mCamera->start();
    return;
}




//******************************************
//gantry

//------------------------------------------
//connect gantry
//proxy function to handle the ConnectGantry and DisconnectGantry functions from MotionHandler
void Magrathea::connectGantryBoxClicked(bool checked)
{
    if (checked)
    {
        if (!mMotionHandler->connectGantry()) {
            ui->connectGantryBox->setChecked(false);
            qWarning("could not connect to gantry");
        }
    } else {
        if (mMotionHandler->xAxisEnabled || mMotionHandler->yAxisEnabled || mMotionHandler->zAxisEnabled || mMotionHandler->uAxisEnabled) {
            ui->connectGantryBox->setChecked(true);
            qWarning("disable axes before disconnecting from gantry");
        } else {
            if(!mMotionHandler->disconnectGantry()) {
                ui->connectGantryBox->setChecked(true);
                qWarning("could not disconnect from gantry");
            }
        }
    }

    //enable/disable buttons according to gantry connection status
    ui->enableAxesButton->setEnabled(mMotionHandler->gantryConnected);
    ui->disableAxesButton->setEnabled(mMotionHandler->gantryConnected);
    ui->xAxisEnableBox->setEnabled(mMotionHandler->gantryConnected);
    ui->yAxisEnableBox->setEnabled(mMotionHandler->gantryConnected);
    ui->zAxisEnableBox->setEnabled(mMotionHandler->gantryConnected);
    ui->uAxisEnableBox->setEnabled(mMotionHandler->gantryConnected);
    ui->resetErrorButton->setEnabled(mMotionHandler->gantryConnected);

    return;
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

    //stop
    ui->stopButton->setEnabled(mMotionHandler->xAxisEnabled ||
                               mMotionHandler->yAxisEnabled ||
                               mMotionHandler->zAxisEnabled ||
                               mMotionHandler->uAxisEnabled);

    //joystick
    ui->leftTabWidget->widget(0)->setEnabled(mMotionHandler->xAxisEnabled ||
                                             mMotionHandler->yAxisEnabled ||
                                             mMotionHandler->zAxisEnabled ||
                                             mMotionHandler->uAxisEnabled);

    //enable/disable buttons according to axes status
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
        mMotionHandler->runX(+1, ui->xAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->runX(-1, ui->xAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->runY(+1, ui->yAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->runY(-1, ui->yAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->runZ(+1, ui->zAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->runZ(-1, ui->zAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->runU(+1, ui->uAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->runU(-1, ui->uAxisSpeedDoubleSpinBox->value());
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
        mMotionHandler->moveXBy(+1*abs(ui->xAxisStepDoubleSpinBox->value()), ui->xAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeXButton)
        mMotionHandler->moveXBy(-1*abs(ui->xAxisStepDoubleSpinBox->value()), ui->xAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveYButton)
        mMotionHandler->moveYBy(+1*abs(ui->yAxisStepDoubleSpinBox->value()), ui->yAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeYButton)
        mMotionHandler->moveYBy(-1*abs(ui->yAxisStepDoubleSpinBox->value()), ui->yAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveZButton)
        mMotionHandler->moveZBy(+1*abs(ui->zAxisStepDoubleSpinBox->value()), ui->zAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeZButton)
        mMotionHandler->moveZBy(-1*abs(ui->zAxisStepDoubleSpinBox->value()), ui->zAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->positiveUButton)
        mMotionHandler->moveUBy(+1*abs(ui->uAxisStepDoubleSpinBox->value()), ui->uAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->negativeUButton)
        mMotionHandler->moveUBy(-1*abs(ui->uAxisStepDoubleSpinBox->value()), ui->uAxisSpeedDoubleSpinBox->value());

    //naviagtion panel
    else if  (sender() == ui->xAxisStepMoveButton)
        mMotionHandler->moveXBy(ui->xAxisStepDoubleSpinBox->value(), ui->xAxisSpeedDoubleSpinBox->value());
    else if  (sender() == ui->yAxisStepMoveButton)
        mMotionHandler->moveYBy(ui->yAxisStepDoubleSpinBox->value(), ui->yAxisSpeedDoubleSpinBox->value());
    else if  (sender() == ui->zAxisStepMoveButton)
        mMotionHandler->moveZBy(ui->zAxisStepDoubleSpinBox->value(), ui->zAxisSpeedDoubleSpinBox->value());
    else if  (sender() == ui->uAxisStepMoveButton)
        mMotionHandler->moveUBy(ui->uAxisStepDoubleSpinBox->value(), ui->uAxisSpeedDoubleSpinBox->value());
    return;
}

//------------------------------------------
//position move proxy function to pass arguments to the MotionHandler
void Magrathea::positionMove()
{
    if (sender() == ui->xAxisPositionMoveButton)
        mMotionHandler->moveXTo(ui->xAxisPositionMoveDoubleSpinBox->value(), ui->xAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->yAxisPositionMoveButton)
        mMotionHandler->moveYTo(ui->yAxisPositionMoveDoubleSpinBox->value(), ui->yAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->zAxisPositionMoveButton)
        mMotionHandler->moveZTo(ui->zAxisPositionMoveDoubleSpinBox->value(), ui->zAxisSpeedDoubleSpinBox->value());
    else if (sender() == ui->uAxisPositionMoveButton)
        mMotionHandler->moveUTo(ui->uAxisPositionMoveDoubleSpinBox->value(), ui->uAxisSpeedDoubleSpinBox->value());
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
