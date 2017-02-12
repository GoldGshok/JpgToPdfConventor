#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPdfWriter>
#include <QFileDialog>
#include <QStringList>
#include <QPainter>
#include <QMessageBox>
#include <QCollator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList getListFiles();
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
};

#endif // MAINWINDOW_H
