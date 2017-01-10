#ifndef PRINTER_H
#define PRINTER_H
#include <QtCore>
#include <QtGui>
#include <QPrinterInfo>

class Printer:public QObject
{
    Q_OBJECT
public:
    Printer();

private:
    void initCups();
    QPrinterInfo mPrinterInfo;
};

#endif // PRINTER_H
