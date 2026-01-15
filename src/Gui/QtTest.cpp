#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QLabel>
#include <QTimer>

int main(int argc, char** argv) {
    if (qEnvironmentVariableIsEmpty("DISPLAY") && qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY")) {
        qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));
    }

    QApplication app(argc, argv);

    QLabel label("Qt 6.6.2 OK");
    label.resize(320, 80);
    label.show();

    QTimer::singleShot(50, &app, &QCoreApplication::quit);

    return app.exec();
}
