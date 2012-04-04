#include "propertymodel.h"

#include <QDebug>


PropertyModel::PropertyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int PropertyModel::rowCount(const QModelIndex & /* parent */) const
{
    return d_propertyMap.size();
}

int PropertyModel::columnCount(const QModelIndex & /* parent */) const
{
    return 2;
}

QVariant PropertyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (index.column())
    {
        case 0:
        {
            return (d_propertyMap.begin() + index.row()).key();
        }

        case 1:
        {
            return (d_propertyMap.begin() + index.row()).value();
        }
    }

    return QVariant();
}

bool PropertyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QString map_key = (d_propertyMap.begin() + index.row()).key();
        d_propertyMap[map_key] = value;
        emit(dataChanged(index, index));

        return true;
    }

    return false;
}

QVariant PropertyModel::headerData(int section, Qt::Orientation  orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            switch (section)
            {
                case 0:
                {
                    return "Property";
                }
                case 1:
                {
                    return "Value";
                }
            }
        }

        return QVariant();
}

void PropertyModel::setPropertyMap(const QMap<QString, QVariant> &map)
{
    d_propertyMap = map;
    reset();
}

QMap<QString, QVariant> PropertyModel::propertyMap()
{
    return d_propertyMap;
}

QString PropertyModel::propertyAt(int offset) const
{
    return (d_propertyMap.begin() + offset).key();
}

Qt::ItemFlags PropertyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
        case 0:
        {
            return QAbstractTableModel::flags(index);
        }
        case 1:
        {
            return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
        }
    }
}
