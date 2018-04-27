#include "magrathea.h"
#include "ui_magrathea.h"
#include "QDesktopWidget"
#include "QDockWidget"
#include "widget.h"
#include "camerawidget.h"

QTextEdit *Magrathea::outputLog = 0;

Magrathea::Magrathea(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::Magrathea)
{
    ui->setupUi(this);

    //------------------------------------------
    //output log
    outputLog = new QTextEdit;
    outputLog->setReadOnly(true);
    outputLog->append("This is the output log.");
    qInfo("This is an info.");

    //------------------------------------------
    //window and menu settings
    setWindowTitle(tr("Magrathea"));
    setUnifiedTitleAndToolBarOnMac(true);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);

    //assign corners to side docks
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);


    //------------------------------------------
    //manage windows and widgets
    setCentralWidget(outputLog);
    createDockWindows();
}

Magrathea::~Magrathea()
{
    delete ui;
}

void Magrathea::createDockWindows()
{
    //dock for camera widget
    QDockWidget *cameraDock = new QDockWidget("Camera", this);
    //cameraDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    camera = new CameraWidget(cameraDock);
    cameraDock->setWidget(camera);
    cameraDock->setMinimumSize(this->width()*0.5, this->height()*2./3.);
    addDockWidget(Qt::TopDockWidgetArea, cameraDock);
    //viewMenu->addAction(speakUpDock->toggleViewAction());

    //dock for SpeakUp widget
    QDockWidget *speakUpDock = new QDockWidget("SpeakUp", this);
    speakUpDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    speakUp = new Widget(speakUpDock);
    speakUpDock->setWidget(speakUp);
    speakUpDock->setMinimumSize(this->width()*0.5, this->height()*0.5);
    addDockWidget(Qt::RightDockWidgetArea, speakUpDock);
    //viewMenu->addAction(speakUpDock->toggleViewAction());
}
