#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnSelectPath, SIGNAL(released()), this, SLOT(setPath()));
    connect(ui->btnConvert, SIGNAL(released()), this, SLOT(createPdf()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPath()
{
    //get directory path
    _path = QFileDialog::getExistingDirectory(this,
                               QString::fromUtf8("Открыть папку"),
                               QDir::currentPath(),
                               QFileDialog::ShowDirsOnly
                               | QFileDialog::DontResolveSymlinks);
    ui->editPath->setText(_path);

    //get list jpg files
    if (!_path.isEmpty())
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

QStringList MainWindow::getListFiles()
{
    QDir dir(_path);
    //set output pdf file name
    _fileName = dir.dirName() + ".pdf";
    //return files name in directory
    return dir.entryList(QStringList("*.*"), QDir::Files, QDir::Name);
}

void MainWindow::createPdf()
{
    QPdfWriter writer(_fileName);
    //add images to pdf file
    QPainter painter(&writer);

    writer.setPageSize(QPagedPaintDevice::A4);

    foreach (QString file, _listFiles)
    {
        QImage image(_path + "/" + file);

        int width;
        int height;

        //set orientation
        if (image.height() > image.width())
        {
            writer.setOrientation(QPageLayout::Portrait);
            width = writer.width();
            height = writer.height();
        }
        else
        {
            writer.setOrientation(QPageLayout::Landscape);
            height = writer.width();
            width = writer.height();
        }

        painter.drawImage(QRect(
                            0,
                            0,
                            width,
                            height),
                            image
                          );

        //create new page
        writer.newPage();
    }
    painter.end();

    QMessageBox msgBox;
    msgBox.setText("The PDF file is created");
    msgBox.exec();
}

