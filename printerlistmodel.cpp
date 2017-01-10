#include "printerlistmodel.h"
namespace EPT {


class PrinterListModelPrivate
{
public:
    PrinterListModelPrivate()
    {
//        int role = Qt::UserRole;
        m_roleNames.insert(Qt::UserRole,"prname");
    }

    QStringList m_list;
    QHash<int ,QByteArray> m_roleNames;

    ~PrinterListModelPrivate()
    {
        m_list.clear();
    }

};



PrinterListModel::PrinterListModel(QObject* parent)
    :QAbstractListModel(parent),
      m_dptr(new PrinterListModelPrivate)
{

}

PrinterListModel::~PrinterListModel()
{
    delete m_dptr;
}


int PrinterListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
//    qDebug()<<"m_list.count()"<<m_dptr->m_list.count();
    return m_dptr->m_list.count();
}

QVariant PrinterListModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<__FUNCTION__<<endl;
    if(index.row()<0 || index.row() >= m_dptr->m_list.count()){
        return QVariant();
    }
//    qDebug()<<"role="<<role<<endl;
    return m_dptr->m_list.at(role-Qt::UserRole);
}

QHash<int,QByteArray> PrinterListModel::roleNames() const
{
    qDebug()<<__FUNCTION__<<endl;
//    qDebug()<<"m_roleNames="<<m_dptr->m_roleNames<<endl;
    return m_dptr->m_roleNames;
}

void PrinterListModel::addPrinterName(const QString &prname)
{
//    qDebug()<<"rowCount()="<<rowCount()<<"QModelIndex()"<<QModelIndex()<<endl;
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_dptr->m_list<<prname;
//    qDebug()<<"pname"<<prname<<endl;
    endInsertRows();
}

void PrinterListModel::reload()
{
    qDebug()<<__FUNCTION__<<endl;
    beginResetModel();
    m_dptr->m_list.clear();
    endResetModel();
}

void PrinterListModel::remove(int index)
{
    beginRemoveRows(QModelIndex(),index,index);
    m_dptr->m_list.removeAt(index);
    endRemoveRows();
}


}
