#include "emindprintdbus.h"
#include <QtDBus/QDBusConnection>
#include <QDebug>
#include "client.h"
namespace EPT {


EmindPrintDbus::EmindPrintDbus(const QString &serviceName,const QString &dbusPath) : QObject()
{
    if(QDBusConnection::sessionBus().registerService(serviceName)){
        qDebug()<<"dbus registerService success"<<endl;
    }else{
        qDebug()<<"dbus registerService failed"<<endl;
    }
    if(QDBusConnection::sessionBus().registerObject(dbusPath,this,QDBusConnection::ExportAllSlots)){
        qDebug()<<"dbus registerObject success"<<endl;
    }else{
        qDebug()<<"dbus registerObject failed"<<endl;
    }

     eclient = Client::instance();
}

EmindPrintDbus::~EmindPrintDbus()
{

}

void EmindPrintDbus::add(const QString &file, const QString &title, bool autoRemove ,const QString &options)
{
    qDebug()<<__FUNCTION__<<endl;
    eclient->load(file,title,options,autoRemove);
}




}
