#include "mainwindow.h"
#include "config.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Config config;

    if (config.readConfig(argc > 1 ? QString(argv[1]) : a.applicationDirPath()))
    {
        w.setJpegPath(config.getJpegPath());
        w.setPdfPath(config.getPdfPath());
    }

    w.show();

    w.setWindowIcon(QIcon(QString(a.applicationDirPath() + "/icon.ico")));

    return a.exec();
}
