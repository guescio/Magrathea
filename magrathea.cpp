#include "magrathea.h"
#include "ui_magrathea.h"
#include "QDesktopWidget"

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

    //set central widget
    setCentralWidget(outputLog);

    //------------------------------------------
    //window and menu settings
    setWindowTitle(tr("Magrathea"));
    setUnifiedTitleAndToolBarOnMac(true);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.8);
}

Magrathea::~Magrathea()
{
    delete ui;
}
