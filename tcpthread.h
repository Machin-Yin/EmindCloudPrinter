#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
namespace EPT {


class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread(QObject* parent=0);
    void run();

signals:
    void error(QTcpSocket::SocketError sockErr);

private:
    QTcpSocket *psocket;
};


}
#endif // TCPTHREAD_H
