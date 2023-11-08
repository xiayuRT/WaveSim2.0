#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "../inc/maincontrol.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Create the control class
    maincontrol control(0);

    // Init the motor before start the UI
    control.motor_init();
    QQmlContext* ctx = engine.rootContext();
    ctx->setContextProperty("fixture", &control);

    // GUI start
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
