#ifndef MAGRATHEA_H
#define MAGRATHEA_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

namespace Ui {
    class Magrathea;
}

class Magrathea : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Magrathea(QWidget *parent = 0);
        ~Magrathea();

        QTextEdit *outputLog;

    private:
        Ui::Magrathea *ui;
};

#endif // MAGRATHEA_H
