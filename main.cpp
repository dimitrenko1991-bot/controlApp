#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtCore/QDir>
#include <control.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

   // system("export DISPLAY=:0.0 &&  xset s off && xset -dpms");

    QQuickView *viewer = new QQuickView;

    // The following are needed to make examples run without having to install the module
    // in desktop environments.
#ifdef Q_OS_WIN
    QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
    QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif
    viewer->engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),
                                      QString::fromLatin1("qml")));
    QObject::connect(viewer->engine(), &QQmlEngine::quit, viewer, &QWindow::close);

    viewer->setTitle(QStringLiteral("QML Oscilloscope"));

    Control _contrl;

    viewer->rootContext()->setContextProperty("_contrl", &_contrl);

    viewer->setSource(QUrl("qrc:/main.qml"));
    viewer->setResizeMode(QQuickView::SizeRootObjectToView);
    viewer->setColor(QColor("#404040"));
    viewer->setFlag(Qt::FramelessWindowHint);

    _contrl.show();
    viewer->showFullScreen();

  //  viewer->show();
  //  viewer->hide();

    _contrl.viewer = viewer;
    _contrl.findLastDate();

    return app.exec();
}
