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

    //set default output path
    _pathOutput = QCoreApplication::applicationDirPath();
    ui->editPathOutput->setText(_pathOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPathInput()
{
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

    ui->editPathInput->setText(_pathInput);

    _listDirs.clear();
    _listDirs = getListDirs(QDir(_pathInput));
}

void MainWindow::setPathOutPut()
{
    //get output directory path
    _pathOutput = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               QDir::currentPath(),
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    ui->editPathOutput->setText(_pathOutput);
}

QStringList MainWindow::getListFiles(QString dirName)
{
    QDir dir(dirName);
    //set output pdf file name
    _fileName = dir.dirName() + ".pdf";
    //return files name in directory
    return dir.entryList(QStringList("*.*"), QDir::Files, QDir::Name);
}

QFileInfoList MainWindow::getListDirs(const QDir& dir)
{
    if (!dir.exists())
        qDebug() << "Error get dir!";

    QFileInfoList list;

    list.append(dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

    foreach (QFileInfo currentDir, list)
    {
        list.append(getListDirs(QDir(currentDir.absoluteFilePath())));
    }

    return list;
}

void MainWindow::createPdf()
{
    foreach (QFileInfo dir, _listDirs)
    {
        _listFiles = getListFiles(dir.absoluteFilePath());
        if (!_listFiles.isEmpty())
        {
            QCollator collator;

            collator.setNumericMode(true);
            std::sort(_listFiles.begin(),
                      _listFiles.end(),
                      [&collator](const QString &file1, const QString &file2)
                      {
                        return collator.compare(file1, file2) < 0;
                      });
        }
        else
        {
            continue;
        }

        try
        {
            QPdfWriter writer(_pathOutput + "/" + _fileName);
            //add images to pdf file
            writer.setPageSize(QPagedPaintDevice::A4);

            QStringList::iterator file = _listFiles.begin();

            int width;
            int height;
            int counter = 0;

            QString filename(dir.absoluteFilePath() + "/" + *file);

            QImage image(filename);
            //set orientation
            if (image.height() > image.width())
            {
                writer.setPageOrientation(QPageLayout::Portrait);
            }
            else
            {
                writer.setPageOrientation(QPageLayout::Landscape);
            }

            width = writer.width();
            height = writer.height();

            QPainter painter(&writer);

            painter.drawImage(QRect(
                        0,
                        0,
                        width,
                        height),
                        image
                      );
            file++;
            counter++;

            ui->statusBar->showMessage(QString("Progress files %1/%2").arg(counter).arg(_listFiles.size()));

            while (file != _listFiles.end())
            {
                filename = QString(dir.absoluteFilePath() + "/" + *file);
                image = QImage(filename);

                //set orientation
                if (image.height() > image.width())
                {
                    writer.setPageOrientation(QPageLayout::Portrait);
                }
                else
                {
                    writer.setPageOrientation(QPageLayout::Landscape);
                }

                width = writer.width();
                height = writer.height();

                //create new pdf page
                writer.newPage();

                painter.drawImage(QRect(
                            0,
                            0,
                            width,
                            height),
                            image
                          );

                counter++;
                file++;

                ui->statusBar->showMessage(QString("Progress files %1/%2").arg(counter).arg(_listFiles.size()));
            }
            painter.end();
        }
        catch (...)
        {
        }
    }

    QMessageBox msgBox;
    msgBox.setText("The procedure is end!");
    msgBox.exec();
}

