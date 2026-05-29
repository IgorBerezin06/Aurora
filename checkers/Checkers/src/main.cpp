#include <QtQuick>
#include <auroraapp.h>
#include "CheckersModel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("Checkers"));

    qmlRegisterType<CheckersModel>("Checkers", 1, 0, "CheckersModel");

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/Checkers.qml")));
    view->show();

    return application->exec();
}
