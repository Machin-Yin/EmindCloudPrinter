#include "cupsbackend.h"
#include <QApplication>
#include <QProcess>
#include <QDir>
CupsBackend::CupsBackend()
{
    QStringList args;
    int numCopies = 1;
    QString jobName("print");
    args << "-P" << printerName();
    args << "-#" << QString("%1").arg(numCopies);
    args << "-T" << jobName;
    args << "-r";
//    if(mDuplexType == DuplexAuto && !duplex){
//        args << "-o sides=one-sided";
//    }

    QString file = QString("%1/.cache/ecp_tmp_%2-print.pdf")
                          .arg(QDir::homePath())
                          .arg(QCoreApplication::applicationPid());
//    args << file.toLocal8Bit();
//    QProcess proc;
//    proc.startDetached("lpr",args);
}

bool CupsBackend::print( const QString &jobName, bool duplex, int numCopies) const
{
//    QStringList args;
//    args << "-P" << printerName();
//    args << "-#" << QString("%1").arg(numCopies);
//    args << "-T" << jobName;
//    args << "-r";
//    if(mDuplexType == DuplexAuto && !duplex){
//        args << "-o sides=one-sided";
//    }
//    QString file = QString("%1/.cache/ecp_tmp_%2-print.pdf")
//                          .arg(QDir::homePath())
//                          .arg(QCoreApplication::applicationPid());
//    args << file.toLocal8Bit();
//    QProcess proc;
//    proc.startDetached("lpr",args);
//    return true;
}

QString CupsBackend::printerName()
{
    return QString("hello");
}
