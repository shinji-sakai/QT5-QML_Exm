#include <QGuiApplication>
#include <QtQml>
#include "configmanager.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    qmlRegisterType<ConfigManager>("utility.config.yaml", 1, 0, "ConfigManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
