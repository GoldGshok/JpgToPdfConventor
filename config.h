#ifndef CONFIG_H
#define CONFIG_H

#include <QString>


class Config
{
public:
    Config();

    void setJpegPath(QString& jpegPath);

    void setPdfPath(QString& pdfPath);

    QString& getJpegPath();
    QString& getPdfPath();

    bool readConfig(QString path = "");

private:
    QString _jpegPath;
    QString _pdfPath;
};

#endif // CONFIG_H
