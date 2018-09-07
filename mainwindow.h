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

	void setJpegPath(QString&);
	void setPdfPath(QString&);

public slots:
		void setPathInput();
		void setPathOutPut();
		void createPdf();

private:
		Ui::MainWindow *ui;

		void fetchDirs(const QDir& dirName, QFileInfoList& list);
		void sortFiles(QStringList& files);
		QStringList getListFiles(QString dirName);

		void showMessage(const char* message);
};

#endif // MAINWINDOW_H
