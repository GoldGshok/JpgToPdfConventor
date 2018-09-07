#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
		ui->setupUi(this);

		connect(ui->btnSelectPathInput, SIGNAL(released()), this, SLOT(setPathInput()));
		connect(ui->btnSelectPathOutput, SIGNAL(released()), this, SLOT(setPathOutPut()));
		connect(ui->btnConvert, SIGNAL(released()), this, SLOT(createPdf()));
}

MainWindow::~MainWindow()
{
		delete ui;
}

void MainWindow::setPathInput()
{
		QString _pathInput = ui->editPathInput->text();
		//get input directory path
		if (_pathInput.isEmpty())
		{
				_pathInput = QFileDialog::getExistingDirectory(this,
															 QString::fromUtf8("Открыть папку"),
															 QCoreApplication::applicationDirPath(),
															 QFileDialog::ShowDirsOnly
															 | QFileDialog::DontResolveSymlinks);
		}
		else
		{
				_pathInput = QFileDialog::getExistingDirectory(this,
															 QString::fromUtf8("Открыть папку"),
															 _pathInput,
															 QFileDialog::ShowDirsOnly
															 | QFileDialog::DontResolveSymlinks);
		}

		setJpegPath(_pathInput);
}

void MainWindow::setPathOutPut()
{
		//get output directory path
		QString _pathOutput = QFileDialog::getExistingDirectory(this,
															 QString::fromUtf8("Открыть папку"),
															 QDir::currentPath(),
															 QFileDialog::ShowDirsOnly
															 | QFileDialog::DontResolveSymlinks);
		setPdfPath(_pathOutput);
}

QStringList MainWindow::getListFiles(QString dirName)
{
		QDir dir(dirName);
		return dir.entryList(QStringList("*.*"), QDir::Files, QDir::Name);
}

void MainWindow::fetchDirs(const QDir& dir, QFileInfoList& list)
{
		QFileInfoList temp;

		temp.append(dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

		if (temp.isEmpty())
		{
				list.append(QFileInfo(dir.absolutePath()));
				return;
		}

		foreach (QFileInfo currentDir, temp)
		{
				fetchDirs(QDir(currentDir.absoluteFilePath()), list);
		}
}

void MainWindow::setJpegPath(QString &path)
{
		ui->editPathInput->setText(path);
}

void MainWindow::setPdfPath(QString &path)
{
	ui->editPathOutput->setText(path);
}

void MainWindow::showMessage(const char* message)
{
	QMessageBox msgBox;
	msgBox.setText(message);
	msgBox.exec();
}

void MainWindow::createPdf()
{
	QFileInfoList listDirs;
	fetchDirs(QDir(ui->editPathInput->text()), listDirs);

	foreach (QFileInfo dir, listDirs)
	{
		QStringList files = getListFiles(dir.absoluteFilePath());
		if (!files.isEmpty())
			sortFiles(files);
		else
			continue;

		QPdfWriter writer(ui->editPathOutput->text() + "/" + dir.baseName() + ".pdf");
		writer.setPageSize(QPagedPaintDevice::A4);
		QPainter painter(&writer);

		try
		{
			for (int i = 0; i < files.size(); ++i)
			{
				QString imageName = QString(dir.absoluteFilePath() + "/" + files.at(i));
				QImage image(imageName, "PNG|JPG|BMP|GIF");

				//set orientation
				image.height() > image.width() ?
							writer.setPageOrientation(QPageLayout::Portrait) :
							writer.setPageOrientation(QPageLayout::Landscape);

				painter.drawImage(QRect(0, 0,
											writer.width(),
											writer.height()),
											image
										);

				if (i != files.size() - 1)
					writer.newPage();

				ui->statusBar->showMessage(QString("Progress files directory %1: %2/%3").
																	 arg(dir.baseName()).
																	 arg(i + 1).
																	 arg(files.size()));
			}
		}
		catch (const std::exception& e)
		{
			showMessage(e.what());
		}

		painter.end();
	}

	showMessage("The procedure is end!");
}

void MainWindow::sortFiles(QStringList &files)
{
	QCollator collator;
	collator.setNumericMode(true);

	std::sort(files.begin(),
						files.end(),
						[&collator](const QString &filePrev, const QString &fileNext)
						{
							return collator.compare(filePrev, fileNext) < 0;
						});
}

