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
    _pathOutput = QDir::absolutePath();
    ui->editPathOutput->setText(_pathOutput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPathInput()
{
    //get input directory path
    _pathInput = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               QDir::currentPath(),
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    ui->editPathInput->setText(_pathInput);

    //get list jpg files
    if (!_pathInput.isEmpty())
    {
        _listFiles = getListFiles();
        QCollator collator;

        collator.setNumericMode(true);
        std::sort(_listFiles.begin(),
                  _listFiles.end(),
                  [&collator](const QString &file1, const QString &file2)
                  {
                    return collator.compare(file1, file2) < 0;
                  });
    }
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

QStringList MainWindow::getListFiles()
{
    QDir dir(_pathInput);
    //set output pdf file name
    _fileName = dir.dirName() + ".pdf";
    //return files name in directory
    return dir.entryList(QStringList("*.*"), QDir::Files, QDir::Name);
}

void MainWindow::createPdf()
{
    try
    {
        QPdfWriter writer(_pathOutput + "/" + _fileName);
        //add images to pdf file
        writer.setPageSize(QPagedPaintDevice::A4);

        QStringList::iterator file = _listFiles.begin();

        int width;
        int height;
        int counter = 0;

        QString filename(_pathInput + "/" + *file);

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
            filename = QString(_pathInput + "/" + *file);
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

        QMessageBox msgBox;
        msgBox.setText("The PDF file is created");
        msgBox.exec();
    }
    catch (...)
    {
    }
}

