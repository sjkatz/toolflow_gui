#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <QAbstractTableModel>

class PropertyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PropertyModel(QObject *parent = 0);

    void setPropertyMap(const QMap<QString, QVariant> &map);
    QMap<QString, QVariant> propertyMap();
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QString propertyAt(int offset) const;

    QMap<QString, QVariant> d_propertyMap;
};

#endif // PROPERTYMODEL_H
