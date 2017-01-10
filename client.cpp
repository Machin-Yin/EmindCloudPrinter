#include "client.h"
#include <QTcpSocket>
#include <QFile>
#include <QProcess>
#include "printerlistmodel.h"
#include <QGuiApplication>


namespace EPT {

const QString AUTH_CODE = "emind";
const quint16 SERVER_PORT = 6666;


class ClientPrive : public QObject {


private:
    ClientPrive(){}

public:
    static ClientPrive *instance();

    QString serverIp;
    QString autstr;
};

ClientPrive* ClientPrive::instance()
{
    static ClientPrive *inst = 0;
    if(!inst)
        inst = new ClientPrive();
    return inst;
}


Client::Client(QObject *parent) : QObject(parent)
{

    priver= ClientPrive::instance();

    psocket = new QTcpSocket();

    connect(psocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    //    connect(psocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    //connect(psocket,SIGNAL(readyRead()),this,SLOT(checkLicense()));
//    connect(psocket,SIGNAL(disconnected()),this,SLOT(resumeConnect()));
//    connect(psocket,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));


    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    printerModel = new PrinterListModel();

    blockSize = 0;
}

Client::~Client()
{
    delete psocket;
    delete printerModel;
}



Client* Client::instance()
{
    static Client *inst = 0;
    if(!inst)
        inst = new Client();
    return inst;
}


void Client::onReadyRead()
{


}

extern QGuiApplication *app111;
void Client::checkConnectivity(QString ip,QString license)
{
    if (priver->serverIp.isEmpty())
        priver->serverIp = ip;

    if (priver->autstr.isEmpty())
        priver->autstr = license;

    //    psocket->abort();

    qDebug()<<"ipstr111=="<<priver->serverIp;
    qDebug()<<"autstr22222=="<<priver->autstr;
    qint64 pid=app111->applicationPid();
    qDebug()<<"pid"<<pid<<endl;
    qDebug()<< __FUNCTION__ <<"this"<<this;

    psocket->connectToHost(ip,SERVER_PORT);
    if(psocket->waitForConnected(1000))
        sndMsg(license);
    if (psocket->waitForReadyRead())
        checkLicense();

}

//license checking process
void Client::sndReqLicense(QString license)
{
    QStringList printer_list;
    sndMsg(license);
    while(psocket->waitForReadyRead()){
        QString bo=rcvMsg();
        qDebug() << "bo==" <<bo;
        if(bo!="OK")
        {
            qDebug()<<"The authcode wrong!"<<endl;
            emit sigAuthWrong();
            return;
        }

        emit sigConnected();
        sndMsg("Request printer list!");
        while(psocket->waitForReadyRead(3000))
        {
            qDebug()<<"waitForReadyRead"<<endl;
            QString printer_str=rcvMsg();
            printer_list = printer_str.split(",");
        }
        foreach (auto printer, printer_list) {
            qDebug()<<"addPrinterName"<<endl;
            printerModel->addPrinterName(printer);
        }
        break;
    }
}

void Client::checkLicense()
{
    QString rmsg=rcvMsg();

    if(!rmsg.isNull())
    {
        if((rmsg.length()<5)&& (rmsg == "OK")){
            connect(psocket,SIGNAL(readyRead()),this,SLOT(checkLicense()));
            emit sigConnected();
            qDebug() << "checkLicense     |" << rmsg << "    |"<< endl;

            reqPrinterList();
        }
        else if(rmsg == "AUTH WRONG"){
            //setErr(rmsg);
            emit sigAuthWrong();
        }else{
            getPrinterNameList(rmsg);
            qDebug() << "getPrinterNameList" << endl;
        }
    }else{

    }
}

void Client::reqPrinterList()
{
    sndMsg("Request printer list!");
}


void Client::setPrinterNameList(const QStringList list)
{
    m_plist = list;
    emit printerNameListChanged();
}

QStringList Client::printerNameList()
{
    return m_plist;
}

void  Client::getPrinterNameList(QString& rmsg)
{
    QString printers = rmsg;

    setPnameStr(printers);

    QStringList tmpList;
    tmpList = printers.split(",");

    for(int i=0;i<tmpList.count();i++) {
        m_plist << tmpList[i];
        printerModel->addPrinterName(tmpList[i]);
    }

    setPrinterNameList(m_plist);
    emit plistSent();
    qDebug()<<"plistSent==>"<<endl;
}

QString Client::pnameStr()
{
    return m_pnameStr;
}

void Client::setPnameStr(const QString str)
{
    m_pnameStr = str;
    emit pnameStrSent(m_pnameStr);
    emit pnameStrChanged();
}

//void Client::setPNameListModel(QStringList modellist)
//{
//    m_pnamelistModel.setStringList(modellist);
//    emit pNameListModelChanged(modellist);
//}

//QStringListModel Client::getPNameListModel()
//{
//    QStringListModel tmodel;
//    tmodel.setStringList(m_plist);
//    return m_pnamelistModel;
//}

void Client::sndMsg(QString msgStr)
{
    qDebug()<<__FUNCTION__<<endl;
    QByteArray authblock;
    QDataStream out(&authblock,QIODevice::WriteOnly);
    out << (quint16)0 << msgStr;
    out.device()->seek(0);
    out<<(quint16)(authblock.size() - sizeof(quint16));
    qDebug()<<"psocket="<<psocket<<endl;
    qDebug()<<"psocket.state="<<psocket->state()<<endl;
    psocket->write(authblock);
    psocket->flush();
}

// Receive short messages : like protocol
QString Client::rcvMsg()
{
    QDataStream in(psocket);

    qDebug()<<psocket->bytesAvailable()<<endl;

    if(blockSize == 0 ){
        if(psocket->bytesAvailable()<(int)sizeof(quint16)){
            return 0;
        }

        in >> blockSize;
    }
    if (psocket->bytesAvailable() < blockSize)
    {
        return 0;
    }

    in >> message;
    blockSize = 0;
    return message;
}

extern QGuiApplication *app111;



void Client::sendFiles(QString fileName)  //实现文件大小等信息的发送
{
    sndMsg("begin send file");
    qDebug()<< __FUNCTION__ <<"this"<<this;
    //    foreach(auto fileName,Files){

    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    //        fileName = localFile->fileName();
    //文件总大小
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);  //将发送缓冲区封闭在一个QDataStream类型的变量中
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    //依次写入总大小信息空间，文件名大小信息空间，文件名---仅是文件名不含路径，没必要含路径
    totalBytes += outBlock.size();

    sendOut << qint64(0) << qint64(0) << currentFileName;
    qint64 filename_size = outBlock.size() - sizeof(qint64) * 2;
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    outBlock.append(localFile->readAll());

    sendOut.device()->seek(0);
    sendOut<<qint64((outBlock.size() - sizeof(qint64)*2));
    sendOut<<qint64(filename_size);

    psocket->write(outBlock);
    //发送完头数据后剩余数据的大小，即文件实际内容的大小
    qDebug()<<"#####totalBytes"<<outBlock.size() - sizeof(qint64) * 2;

    psocket->waitForBytesWritten();
    outBlock.resize(0);

}

void Client::updateClientProgress(qint64 numBytes) //更新进度条，实现文件的传送
{
    qDebug()<<"#######已发送："<<bytesWritten<<"剩余："<<bytesToWrite;
    bytesWritten += (int)numBytes;
    //已经发送数据的大小
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        //初始化时loadSize = 64*1024;qMin为返回参数中较小的值，每次最多发送64K的大小
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
      //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
      //就发送剩余数据的大小
        bytesToWrite -= (int)psocket->write(outBlock);
       //发送完一次数据后还剩余数据的大小
        outBlock.resize(0);
       //清空发送缓冲区
    }
    else
    {
        localFile->close(); //如果没有发送任何数据，则关闭文件
    }
    if(bytesWritten == totalBytes) //发送完毕
    {
        qDebug()<<"SendFiles over"<<endl;
        localFile->close();
//        psocket->close();
    }
}


QString Client::getErr()
{
    return m_err;
}
void Client::setErr(const QString error)
{
    if(error != m_err){
        m_err = error;
        emit errChanged();
    }

}

void Client::update()
{

}

void Client::displayError(QAbstractSocket::SocketError psocketErr)
{
    switch(psocketErr){
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"HostNotFoundError"<<endl;
        emit sigConnectRefused();
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<"ConnectionRefusedError"<<endl;
        emit sigConnectRefused();
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<"psocketTimeoutError"<<endl;
        //        emit sigConnectRefused();
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<"DatagramTooLargeError"<<endl;
        //        emit sigConnectRefused();
        break;
    default:
        break;
    }
}


void Client::printerName() const
{

}

void Client::load(const QString &fileName,const QString &title,const QString &options,bool autoRemove)
{
    qDebug()<<__FUNCTION__<<endl;
    loadCupsFiles(QStringList()<<fileName,QStringList()<<title,options,autoRemove);
}

void Client::loadCupsFiles(const QStringList& fileNames,const QStringList& titles,const QString& options,bool autoRemove)
{
    qDebug()<<"psocket.state="<<psocket->state()<<endl;
    //    emit rcvCupsFile();
    // if(authflags==1)
    //    {
    //        if(psocket->state() == QAbstractSocket::UnconnectedState)
    //            client->checkConnectivity(priver->serverIp,priver->autstr);
    //    }
    foreach(QString fileName,fileNames)
    {
        sendFiles(fileName);
        authflags = 1;
        qDebug()<<__FUNCTION__<<"fileName="<<fileName<<endl;
    }
}

void Client::resumeConnect()
{
    qApp->quit();
    //    if(psocket->state() == QAbstractSocket::UnconnectedState)
    client->checkConnectivity(priver->serverIp,priver->autstr);
}

void Client::setDefaultPrinter(QString prName,quint32 pIndex)
{
    qDebug()<<"pIndex="<<pIndex<<endl;
    //    emit stopIndicator();  //stop qml indicator
    sndMsg("DefaultPrinter");
    sndMsg(QString::number(pIndex));
    //    qDebug()<<"DefaultPrinter pIndex="<<pIndex<<endl;
    //    qDebug()<<prName<<endl;
    //    qDebug()<<__FUNCTION__<<endl;
    //    QProcess proc;
    //    QString printerIp(priver->serverIp);
    //    QString ppdName("cups/emindprinter.ppd");
    //   proc.start(QString("gksu -D AddPrinter lpadmin -p %1@%2 -P %3 ").arg(prName).arg(printerIp).arg(ppdName));
    //    proc.start(QString("gksu -D AddPrinter -P  lpadmin  -p emindprinter-3 -v emindprinter:/ -P lsb/usr/emindprinter/emindprinter.ppd"));
    //    emit printerSetFinished();
    //    proc.waitForFinished();


}



}
