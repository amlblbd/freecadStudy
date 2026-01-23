#include <QApplication>
#include <QDebug>
#include <QString>

#include <IFSelect_ReturnStatus.hxx>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>

#include "OccView.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qInfo() << "Qt runtime version:" << qVersion();
    qInfo() << "Qt compile-time version:" << QT_VERSION_STR;

    OccView view;
    view.resize(900, 700);
    view.show();

    if (argc > 1)
    {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        STEPControl_Reader reader;
        IFSelect_ReturnStatus status = reader.ReadFile(filePath.toLocal8Bit().constData());
        if (status == IFSelect_RetDone)
        {
            reader.TransferRoots();
            TopoDS_Shape shape = reader.OneShape();
            view.displayShape(shape);
        }
        else
        {
            qWarning() << "Failed to read STEP file:" << filePath;
        }
    }
    else
    {
        qInfo() << "Usage: MyFreeCAD <path-to-step-file>";
    }

    return app.exec();
}
