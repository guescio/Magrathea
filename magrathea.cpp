#include "magrathea.h"
#include "ui_magrathea.h"
#include "QDesktopWidget"
#include "QDockWidget"
#include "widget.h"

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
    //create widget dock for SpeakUp
    QDockWidget *speakUpDock = new QDockWidget("SpeakUp", this);
    speakUpDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    speakUp = new Widget(speakUpDock);
    speakUpDock->setWidget(speakUp);
    addDockWidget(Qt::RightDockWidgetArea, speakUpDock);
    //viewMenu->addAction(speakUpDock->toggleViewAction());
}
