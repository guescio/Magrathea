#include "widget.h"
#include "ui_widget.h"
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    process = new QProcess;

    connect(ui->testButton,
            SIGNAL(clicked()),
            this,
            SLOT(testButtonClicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    process->start("say", QStringList() << ui->lineEdit->text());
    process->waitForFinished(-1);
    qDebug() << "say" << ui->lineEdit->text();
}

void Widget::testButtonClicked()
{
    process->start("say test");
    process->waitForFinished(-1);
    qDebug() << "say \"test\"";
}
