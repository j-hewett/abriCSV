#include "csvtablemodel.h"
#include <QFile>
#include <QTextStream>

CSVTableModel::CSVTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

void CSVTableModel::loadCSV(const QString &filePath)
{
    beginResetModel();
    m_data.clear();
    m_headers.clear();

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        bool firstLine = true;
        while (!in.atEnd())
        {
            QStringList row = in.readLine().split(',');
            if (firstLine)
            {
                m_headers = row;
                firstLine = false;
            }
            else
            {
                m_data.append(row);
            }
        }
    }
    endResetModel();
}

QVariant CSVTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};
    return {};
}

int CSVTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_rowCount;
}

int CSVTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_columnCount;
}

QVariant CSVTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    const QStringList &row = m_data.at(index.row());
    if (index.column() < row.size())
        return row.at(index.column()).trimmed();
    return QVariant();
}
