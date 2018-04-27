#include "magrathea.h"
#include "ui_magrathea.h"
#include "qtextedit.h"

Magrathea::Magrathea(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Magrathea)
{
    ui->setupUi(this);

    //------------------------------------------
    //output log
    outputLog=new QTextEdit;
    outputLog->setReadOnly(true);

    //set central widget
    setCentralWidget(outputLog);

    //------------------------------------------
    //window and menu settings
    setWindowTitle(tr("Dock Widgets"));
    setUnifiedTitleAndToolBarOnMac(true);
}

Magrathea::~Magrathea()
{
    delete ui;
}
