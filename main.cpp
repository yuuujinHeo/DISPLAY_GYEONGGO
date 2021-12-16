#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QCursor>

#include "Backend.h"
#include "AdminConnector.h"

QObject *object;
QObject *objectLeft;
QObject *objectRight;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    app.setOverrideCursor(QCursor(Qt::BlankCursor));
    qmlRegisterType<Backend>("io.qt.Backend", 1, 0, "Backend");
    qmlRegisterType<AdminConnector>("io.qt.AdminConnector", 1, 0, "AdminConnector");

    qDebug() <<"main";
    QQmlEngine engine;
    QQmlComponent component(&engine, "qrc:/OneView.qml");
    object = component.create();

//    QQmlEngine engine1;
//    QQmlComponent component1(&engine1, "qrc:/LeftView.qml");
//    objectLeft = component1.create();

//    QQmlEngine engine2;
//    QQmlComponent component2(&engine2, "qrc:/RightView.qml");
//    objectRight = component2.create();

    return app.exec();
}
