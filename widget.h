#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class QProcess;

class Widget : public QWidget
{
    Q_OBJECT

    public:
        explicit Widget(QWidget *parent = 0);
        ~Widget();

    private slots:
        void on_pushButton_clicked();
        void testButtonClicked();

    private:
        Ui::Widget *ui;
        QProcess *process;
};

#endif // WIDGET_H
