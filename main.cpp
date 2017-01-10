#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "client.h"
#include <QMetaObject>
#include <QVariant>
#include <QString>
#include "cupsbackend.h"
#include "printer.h"
#include <QQuickView>
#include <QQmlContext>
#include <QtQuick/QQuickItem>
#include "emindprintdbus.h"
#include "printerlistmodel.h"
#include <QTranslator>
#include "clientassistant.h"

#define TRANSLATIONS_DIR "/usr/share/emindprinter/translations/"

QGuiApplication *app111=NULL;


using namespace EPT;

void readEnvFile()
{
    QFile envFile(QDir::homePath() + "/.cache/emindprinter.env");
    if(envFile.exists()){
        envFile.open(QFile::ReadOnly);
        while(!envFile.atEnd()){
            QString line = QString::fromLocal8Bit(envFile.readLine());
            QString name = line.section("=",0,0).trimmed();
            QString value = line.section("=",1).trimmed();
            if(value.startsWith('\'')&&value.endsWith('\'') || (value.startsWith('"')&&value.startsWith('"'))){
                value = value.mid(1,value.length()-2);
            }

            qputenv(name.toLocal8Bit(),value.toLocal8Bit());
        }
        envFile.close();
    }
}

int main(int argc, char *argv[])
{
    readEnvFile();
    QGuiApplication app(argc, argv);
    app111 = &app;



    //client register
    qmlRegisterType<ClientAssistant>("com.client.emindprint",1,0,"ClientAssistant");

    qmlRegisterType<Client>("com.client.emindprint",1,0,"EmindClient");
    //    qmlRegisterType<CupsBackend>("com.client.emindprint",1,0,"CupsBackend");
    //    qmlRegisterType<Printer>("com.client.emindprint",1,0,"Printer");
    qmlRegisterType<PrinterListModel>("com.client.emindprint",1,0,"PrinterListModel");

    //translations
    QTranslator trans;
    if(trans.load(TRANSLATIONS_DIR"zh_CN.qm"))
        app.installTranslator(&trans);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    QObject::connect(&engine,SIGNAL(quit()),qApp,SLOT(quit()));

    QObject* root = NULL;
    QList<QObject*> rootObjs = engine.rootObjects();
    for(int i=0;i<rootObjs.size();i++){
        if(rootObjs.at(i)->objectName() == "rootObject"){
            root = rootObjs.at(i);
            break;
        }
    }

    //to handle cups files
    QStringList files;
    QStringList titles;
    QStringList args = app.arguments();
    bool autoRemove = false;

    for(int i=1;i < args.count();++i){
        QString arg = args.at(i);
        if(arg == "-t" || arg == "--title"){
            if(i+1 < args.count()){
                titles<<args.at(i+1);
                i++;
                continue;
            }else{
                return 1;
            }
        }

        files << args.at(i);
    }

    EmindPrintDbus emDbus("org.emindprinter","/emindprinter");

    app.processEvents();

    client->loadCupsFiles(files,titles,"",autoRemove);

    return app.exec();
}


