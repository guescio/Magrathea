#ifndef MAGRATHEA_H
#define MAGRATHEA_H

#include <QMainWindow>
#include <QTextEdit>

namespace Ui {
    class Magrathea;
}

class QTextEdit;

class Magrathea : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Magrathea(QWidget *parent = 0);
        ~Magrathea();

        static QTextEdit *outputLog;

    private:
        Ui::Magrathea *ui;
        //QTextEdit *outputLog;
};

#endif // MAGRATHEA_H
