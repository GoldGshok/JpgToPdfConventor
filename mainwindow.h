#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPdfWriter>
#include <QFileDialog>
#include <QStringList>
#include <QPainter>
#include <QMessageBox>
#include <QCollator>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList getListFiles(QString dirName);
    void getListDirs(const QDir& dirName, QFileInfoList& list);

    void setJpegPath(QString&);
    void setPdfPath(QString&);

public slots:
    void setPathInput();
    void setPathOutPut();
    void createPdf();

private:
    Ui::MainWindow *ui;
    QString _pathInput;
    QString _pathOutput;
    QString _fileName;
    QStringList _listFiles;
    QFileInfoList _listDirs;
};

#endif // MAINWINDOW_H
