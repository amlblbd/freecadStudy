#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QtPlugin>

class QXcbIntegrationPlugin;
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)

int main(int argc, char** argv) {
    if (qEnvironmentVariableIsEmpty("DISPLAY") && qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    } else if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    }

    QApplication app(argc, argv);

    qInfo() << "Qt runtime version:" << qVersion();
    qInfo() << "Qt compile-time version:" << QT_VERSION_STR;

    QLabel label("Qt 6.6.2 OK");
    label.resize(320, 80);
    label.show();

    QTimer::singleShot(50, &app, &QCoreApplication::quit);

    return app.exec();
}
