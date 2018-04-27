#include "magrathea.h"
#include <QApplication>

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "DEBUG: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLog != 0)
            message.sprintf("DEBUG: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLog->append(message);
        break;
    case QtInfoMsg:
        fprintf(stderr, "INFO: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLog != 0)
            //message.sprintf("INFO: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            message.sprintf("%s", localMsg.constData());
            Magrathea::outputLog->append(message);
        break;
    case QtWarningMsg:
        fprintf(stderr, "WARNING: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLog != 0)
            //message.sprintf("WARNING: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            message.sprintf("WARNING: %s", localMsg.constData());
            Magrathea::outputLog->append(message);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRITICAL: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLog != 0)
            message.sprintf("CRITICAL: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLog->append(message);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        if(Magrathea::outputLog != 0)
            message.sprintf("FATAL: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
            Magrathea::outputLog->append(message);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    QApplication app(argc, argv);
    Magrathea mainWindow;
    mainWindow.show();

    return app.exec();
}
