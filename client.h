#ifndef CLIENT_H
#define CLIENT_H

#include <QMessageBox>
#include <QObject>
#include <QAbstractSocket>
#include <QStringListModel>
#include "tcpthread.h"
class QFile;
class QTcpSocket;

namespace EPT {

class PrinterListModel;

class ClientPrive;
class Client : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QStringListModel pNameListModel READ getPNameListModel NOTIFY pNameListModelChanged)
    Q_PROPERTY(QString err READ getErr WRITE setErr NOTIFY errChanged)
    Q_PROPERTY(QStringList printerNameList READ printerNameList WRITE setPrinterNameList NOTIFY printerNameListChanged)
    Q_PROPERTY(QString pnameStr READ pnameStr WRITE setPnameStr NOTIFY pnameStrChanged)
    Q_ENUMS(ServerError)


public:
    explicit Client(QObject *parent = 0);
    ~Client();

    static Client* instance();


    ClientPrive* priver;
    enum ServerError{ IpError,LicenseError};

    Q_INVOKABLE void update();
    Q_INVOKABLE void reqPrinterList();
    Q_INVOKABLE void sndReqLicense(QString license);
    Q_INVOKABLE void sendFiles(QString fileName);

    QString pnameStr();
    void setPnameStr(const QString);
    QString getErr();
    void setErr(const QString);
    QStringList printerNameList();
    void setPrinterNameList(const QStringList);
    void printerName() const;
    void load(const QString &fileName,const QString &title,const QString &options,bool autoRemove);

    void loadCupsFiles(const QStringList& fileNames,const QStringList& titles,const QString& options,bool autoRemove);
    void sndMsg(QString msgStr);
    QString rcvMsg();
    void checkConnect();


//    void setPNameListModel(QStringList modellist);
//    QStringListModel getPNameListModel();

signals:
    void error();
    void errChanged();
    void errConnected();
    void printerNameListChanged();
    void pNameListModelChanged(QStringList);
    void rcvCupsFile();
    void sigConnected();
    void sigAuthWrong();
    void sigConnectRefused();
    void pnameStrChanged();
    void plistSent();
    void printerSetFinished();
    void stopIndicator();
    void pnameStrSent(QString);

public slots:
    void setDefaultPrinter(QString prName,quint32 pIndex);
    void getPrinterNameList(QString& msg);
//    Q_INVOKABLE void checkConnectivity(QString ip,QString license);
    void checkConnectivity(QString ip,QString license);
    void checkLicense();
    void displayError(QAbstractSocket::SocketError);
    void updateClientProgress(qint64 numBytes);
    void onReadyRead();
    void resumeConnect();



private:
    QTcpSocket *psocket;

    quint16 inDataSize;

    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小,即文件实际内容的大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径

    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    quint16 blockSize;

    QString message;
    int authflags = 0;

    QString m_err;
    QStringList m_plist;
    QStringListModel m_pnamelistModel;

    PrinterListModel *printerModel;
    QString m_pnameStr;
    quint16 printerNo;


};


#define client EPT::Client::instance()


}

#endif // CLIENT_H
