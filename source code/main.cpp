#include "main.h"
#include "backend.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    // call the fucntion canvas refresh in qml file when signal pathsChangel is toggled on
    QObject::connect(&backend, &Backend::pathsChanged, [&]() {
        QList<QObject*> rootObjects = engine.rootObjects();
        if (!rootObjects.isEmpty()) {
            QObject *rootObject = rootObjects.first();
            QMetaObject::invokeMethod(rootObject, "refreshCanvas", Qt::QueuedConnection);
        }
    });


    // call the slot do_something when the button is pressed
    QList<QObject*> rootObjects = engine.rootObjects();
    if (!rootObjects.isEmpty()) {
        QObject *rootObject = rootObjects.first();

        // Find the button by its objectName and connect it to the slot
        QObject *button = rootObject->findChild<QObject*>("findPathButton");
        if (button) {
            QObject::connect(button, SIGNAL(clicked()), &backend, SLOT(do_something()));
        }
    }

    return app.exec();
}
