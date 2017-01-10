#include "printer.h"
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrinter>
#include <QExplicitlySharedDataPointer>
#include <cups/cups.h>
#include <cups/ppd.h>
Printer::Printer()
{

}

void Printer::initCups()
{
    cups_dest_t *dests;
    int numDests = cupsGetDests(&dests);
    cups_dest_t *dest = cupsGetDest(mPrinterInfo.printerName().toLocal8Bit().data(),0,numDests,dests);

#if 1
    qDebug() << "**" << mPrinterInfo.printerName() << "*******************";
    for (int j=0; j<dest->num_options; ++j)
        qDebug() << "  *" << dest->options[j].name << dest->options[j].value;
#endif

}
