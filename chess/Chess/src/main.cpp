#include <QtQuick>
#include <auroraapp.h>
#include "boardmodel.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("Chess"));

    qmlRegisterType<BoardModel>("Chess", 1, 0, "BoardModel");

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/Chess.qml")));
    view->show();

    return application->exec();
}
