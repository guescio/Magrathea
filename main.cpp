#include "magrathea.h"
#include <QApplication>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "DEBUG: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLogTextEdit != 0)
            message.sprintf("DEBUG: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLogTextEdit->append(message);
        break;
    case QtInfoMsg:
        fprintf(stderr, "INFO: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLogTextEdit != 0)
            //message.sprintf("INFO: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            message.sprintf("%s", localMsg.constData());
            Magrathea::outputLogTextEdit->append(message);
        break;
    case QtWarningMsg:
        fprintf(stderr, "WARNING: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLogTextEdit != 0)
            message.sprintf("WARNING: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLogTextEdit->append(message);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRITICAL: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLogTextEdit != 0)
            message.sprintf("CRITICAL: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLogTextEdit->append(message);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLogTextEdit != 0)
            message.sprintf("FATAL: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLogTextEdit->append(message);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    QApplication a(argc, argv);
    Magrathea w;
    w.show();

    return a.exec();
}

QTextEdit *Magrathea::outputLogTextEdit = 0;
