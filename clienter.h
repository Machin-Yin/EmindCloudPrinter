#ifndef CLIENTER_H
#define CLIENTER_H

#include <QObject>

class Clienter : public QObject
{
    Q_OBJECT
public:
    explicit Clienter(QObject *parent = 0);

signals:

public slots:
};

#endif // CLIENTER_H