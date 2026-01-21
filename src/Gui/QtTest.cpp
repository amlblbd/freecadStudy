#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QtPlugin>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qInfo() << "Qt runtime version:" << qVersion();
    qInfo() << "Qt compile-time version:" << QT_VERSION_STR;

    // 最小窗口：能显示出来（或 offscreen 下成功初始化）就说明 Widgets + 平台插件链路正常
    QLabel label(QString("Qt ") + qVersion() + " OK");
    label.resize(320, 80);
    label.show();

    return app.exec();
}
