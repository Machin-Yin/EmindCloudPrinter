#include "clientassistant.h"

ClientAssistant::ClientAssistant(QObject* parent) : QObject(parent)
{
    connect(this,SIGNAL(msgClient(QString,QString)),client,SLOT(checkConnectivity(QString,QString)));
    connect(client,SIGNAL(sigConnected()),this,SIGNAL(sigConnected()));
    connect(client,SIGNAL(sigAuthWrong()),this,SIGNAL(sigAuthWrong()));
    connect(client,SIGNAL(sigConnectRefused()),this,SIGNAL(sigConnectRefused()));
    connect(client,SIGNAL(plistSent()),this,SIGNAL(plistSent()));
    connect(client,SIGNAL(printerSetFinished()),this,SIGNAL(printerSetFinished()));
    connect(client,SIGNAL(pnameStrSent(QString)),this,SLOT(pnameStrRcved(QString)));
    connect(this,SIGNAL(sigSetDefaultPrnter(QString,quint32)),client,SLOT(setDefaultPrinter(QString,quint32)));
}

void ClientAssistant::checkConnectivity(QString ip,QString license)
{
     emit msgClient(ip,license);
}

ClientAssistant::~ClientAssistant()
{

}

void ClientAssistant::pnameStrRcved(QString pnamelist)
{
    qDebug()<<__FUNCTION__<<endl;
    setPnameStr(pnamelist);
}

QString ClientAssistant::pnameStr()
{
    return m_pnameStr;
}

void ClientAssistant::setPnameStr(const QString str)
{
    m_pnameStr = str;
    emit pnameStrChanged();
}


void ClientAssistant::setDefaultPrinter(QString prName, quint32 pIndex)
{
    emit sigSetDefaultPrnter(prName,pIndex);
}
