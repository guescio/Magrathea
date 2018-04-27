#ifndef MAGRATHEA_H
#define MAGRATHEA_H

#include <QMainWindow>
#include <QTextEdit>

namespace Ui {
    class Magrathea;
}

class QTextEdit;
class Widget;
class CameraWidget;

class Magrathea : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Magrathea(QWidget *parent = 0);
        ~Magrathea();

        static QTextEdit *outputLog;

    private:
        void createDockWindows();

        Ui::Magrathea *ui;
        Widget *speakUp;
        CameraWidget *camera;
};

#endif // MAGRATHEA_H
