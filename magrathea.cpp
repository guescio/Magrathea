#include "magrathea.h"
#include "ui_magrathea.h"

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
}

Magrathea::~Magrathea()
{
    delete ui;
}
