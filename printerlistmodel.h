#ifndef PRINTERLISTMODEL_H
#define PRINTERLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QtCore>
#include <QHash>
#include <QByteArray>
namespace EPT {


class PrinterListModelPrivate;
class PrinterListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    PrinterListModel(QObject* parent=0);
    ~PrinterListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    Q_INVOKABLE void addPrinterName(const QString& pname);

    Q_INVOKABLE void reload();
    Q_INVOKABLE void remove(int index);

protected:

    QHash<int,QByteArray> roleNames() const ;



private:
    PrinterListModelPrivate *m_dptr;
};
}

#endif // PRINTERLISTMODEL_H
