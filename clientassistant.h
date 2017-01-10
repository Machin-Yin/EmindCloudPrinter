#ifndef CLIENTASSISTANT_H
#define CLIENTASSISTANT_H

#include "client.h"
#include <QObject>



class ClientAssistant : public QObject
{
    Q_OBJECT

public:
    explicit ClientAssistant(QObject* parent=0);
    ~ClientAssistant();

    Q_INVOKABLE void checkConnectivity(QString ip,QString license);
    Q_PROPERTY(QString pnameStr READ pnameStr WRITE setPnameStr NOTIFY pnameStrChanged)
    QString pnameStr();
    void setPnameStr(const QString);
    Q_INVOKABLE void setDefaultPrinter(QString prName,quint32 pIndex);


signals:
    void msgClient(QString ,QString );
    void sigConnected();
    void sigAuthWrong();
    void sigConnectRefused();
    void plistSent();
    void sigStopIndicator();
    void printerSetFinished();
    void pnameStrChanged();
    void sigSetDefaultPrnter(QString,quint32);

public slots:
    void pnameStrRcved(QString);


private:

    QString m_pnameStr;

};

#endif // CLIENTASSISTANT_H
