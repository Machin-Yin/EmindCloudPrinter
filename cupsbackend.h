#ifndef CUPSBACKEND_H
#define CUPSBACKEND_H

#include <QObject>

class CupsBackend:public QObject
{
    Q_OBJECT
public:
    CupsBackend();
    virtual bool print(const QString &jobName, bool duplex, int numCopies = 1) const;
    QString printerName();

};

#endif // CUPSBACKEND_H
