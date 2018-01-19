#include "config.h"
#include <QSettings>
#include <QDir>

Config::Config()
{}

void Config::setJpegPath(QString &jpegPath)
{
    _jpegPath = jpegPath;
}

void Config::setPdfPath(QString &pdfPath)
{
    _pdfPath = pdfPath;
}

QString &Config::getJpegPath()
{
    return _jpegPath;
}

QString &Config::getPdfPath()
{
    return _pdfPath;
}

bool Config::readConfig(QString path)
{
    try
    {
        if (path.isEmpty())
            throw;

        path += "/config.ini";

        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("path");

        _jpegPath = settings.value("JPEG_PATH").toString();
        _pdfPath = settings.value("PDF_PATH").toString();

        settings.endGroup();
    }
    catch (...)
    {
        return false;
    }

    return true;
}
